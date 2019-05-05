#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>
#include <ctype.h>

int main(){
    char buffer[100];
    read(STDIN_FILENO, buffer, 100);
    if(read(STDIN_FILENO, buffer, 100)==EOF){
        write(STDOUT_FILENO,"jaja\n",6);
    }
    else{
        write(STDOUT_FILENO, "chao\n",6);
    }
    return 0;
}