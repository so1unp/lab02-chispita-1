#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    char *mensaje = argv[1];

    printf("hola %s\n", mensaje);
    
    // Termina la ejecución del proceso.
    exit(EXIT_SUCCESS);
}