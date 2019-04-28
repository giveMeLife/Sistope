#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#define LECTURA 0
#define ESCRITURA 1


int main(){
    printf("jaja\n");
    char buffer[100];
    printf("hola del hijo");
    read(STDIN_FILENO, buffer, 100);
    printf("Hijo: %s", buffer);
    write(STDOUT_FILENO, "TE ENVIO ESTE MENSAJE CUALQUIERA\n", 34);
    exit(1);
    return 0;
}