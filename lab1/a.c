#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#define LECTURA 0
#define ESCRITURA 1


int main(){
    char buffer[100];
    read(STDIN_FILENO, buffer, 100);
    printf("%s",buffer);
    exit(1);
    return 0;
}