/*
 * Copia un archivo en otro usando llamadas al sistema o funciones de biblioteca.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>     // exit()
#include <fcntl.h>      // open() y macros O_*
#include <unistd.h>     // read(), write(), close()

int main(int argc, char *argv[])
{
    // validación de argumentos 
    if (argc < 4) {
        printf("¡Faltan aclarar argumentos!\n");
        exit(EXIT_FAILURE);
    } else if (argc > 4) {
        printf("¡Demasiados argumentos!\n");
        exit(EXIT_FAILURE);
    }

    char *opcion = argv[1]; 
    int tam = strlen(opcion); 
    char *nombre_origen = argv[2];
    char *nombre_destino = argv[3];

    char texto[1000]; 
    int bytes;
    ssize_t leer;

    // validacion de la opcion
    if ((*opcion != 's' && *opcion != 'f') || tam != 1) {
        printf("¡Opción no admitida! (selecciona \"s\" o \"f\")\n");
        exit(EXIT_FAILURE);
    }

    /* CON SYSCALLS */
    if (*opcion == 's') {
        int fd_origen = open(nombre_origen, O_RDONLY);
        if (fd_origen == -1) {
            perror("¡El archivo origen no existe!");
            exit(EXIT_FAILURE);
        }

        int fd_destino = open(nombre_destino, O_WRONLY | O_CREAT | O_EXCL, 0644);
        if (fd_destino == -1) {
            perror("¡El archivo destino ya existe!");
            close(fd_origen);
            exit(EXIT_FAILURE);
        }

        while ((leer = read(fd_origen, texto, sizeof(texto))) > 0) {
            int total = 0;
            while (total < leer) {
                int escribir = write(fd_destino, texto + total, leer - total);
                if (escribir == -1) {
                    perror("¡Error al escribir!");
                    close(fd_origen);
                    close(fd_destino);
                    exit(EXIT_FAILURE);
                }
                total += escribir;
            }
        }

        close(fd_origen);
        close(fd_destino);

    /* CON LIBRERÍAS */
    } else if (*opcion == 'f') {
        FILE *file_origen = fopen(nombre_origen, "r");
        if (file_origen == NULL) {
            perror("¡El archivo origen no existe!");
            exit(EXIT_FAILURE);
        }
        FILE *file_destino = fopen(nombre_destino, "wx"); 
        if (file_destino == NULL) {
            perror("¡El archivo destino ya existe!");
            fclose(file_origen); 
            exit(EXIT_FAILURE);
        }

        while ((bytes = fread(texto, sizeof(char), sizeof(texto), file_origen)) > 0) {
            fwrite(texto, sizeof(char), bytes, file_destino);
        }

        fclose(file_origen);
        fclose(file_destino);
    }    

    exit(EXIT_SUCCESS);
}