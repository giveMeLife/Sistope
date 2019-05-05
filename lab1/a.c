#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>
#include <ctype.h>
int **pipesI;
int **pipesO;

int main(){
    int n = 9;
    int pipesI[n][2];
    int pipesO[n][2];
    int childp[n];
    for(int i = 0; i<n; i++){
        pipe(pipesI[i]);
        pipe(pipesO[i]);
    }
    for(int i = 0; i<n; i++){
        childp[i] = fork();
        if(childp[i]==0){
            dup2(pipesI[i][0],STDIN_FILENO);
            dup2(pipesO[i][1],STDOUT_FILENO);
            close(pipesI[i][1]);
            close(pipesO[i][0]);
            execl("./i","./i",(char *)NULL);
        }
        else{
            close(pipesI[i][0]);
            close(pipesO[i][1]);
        }
    }
    char buffer[100];
    for(int i = 0; i<n; i++){
        write(pipesI[i][1],"hola\n",6);
        close(pipesI[i][1]);
    }
    for(int i = 0; i<n; i++){
        read(pipesO[i][0],buffer,100);
        printf("me mandaron: %s\n",buffer);
    }

    return 0;
}
