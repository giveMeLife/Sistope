#include <stdio.h>
#include <stdlib.h>

int main(){
    float* a = malloc(sizeof(float)*3);
    a = NULL;
    if(a == NULL){
        printf("hola\n");
    }
    a = malloc(sizeof(float)*3);
    printf("%f\n", a[1]);
    return 0;
}