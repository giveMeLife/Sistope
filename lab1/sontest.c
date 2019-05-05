#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>
#include <ctype.h>



#define LECTURA 0
#define ESCRITURA 1



typedef struct Nodo{
	float v;
	float u;
	float w;
	float i;
	float r;
	struct Nodo*sig;

}Nodo;

typedef struct Lista{
	Nodo * inicio;
	Nodo * fin;

	
}Lista;

void inicializar(Lista * lista){
	lista->inicio = NULL;
	lista->fin = NULL;
}


Lista*agregarNodo(Lista*lista, float v, float u, float w, float i, float r){
	Nodo*nodo;
	nodo=(Nodo*)malloc(sizeof(Nodo));
	nodo->v=v;
	nodo->u=u;
	nodo->w=w;
	nodo->i=i;
	nodo->r=r;

	nodo->sig=NULL;
	if(lista==NULL){
		lista->inicio=nodo;
		lista->fin = nodo;
	}
	else{
		nodo->sig=lista->inicio;
		lista->inicio=nodo;
	}
	return lista;
}

int largo(Lista* lista){
    int i = 0;
    Nodo * actual = lista->inicio;
    while(actual!=NULL){
		actual = actual->sig;
        i++;
    }
    return i;
}

//N corresponde a las visibilidades del disco
float propiedades(Lista* lista, int tipo, int N){
	float pot = 0;
	float sumW = 0;
	float sumI = 0;
	float sumR = 0;

	Nodo * actual = lista->inicio;

	while(actual!=NULL){
		sumW = sumW + actual->w;
		sumI = sumI + actual->i;		
		sumR = sumR + actual->r;
		pot = pot + sqrt(pow(actual->r,2) + pow(actual->u,2));
		actual = actual->sig;


	}
	

	//Media Real
	if(tipo == 0)
		return sumR/N;
	//Media imaginaria
	else if(tipo == 1)
		return sumI/N;
	//Potencia
	else if(tipo == 2)
		return pot;
	else
		return sumW;

}

//Verifica si el padre dejará de mandar datos o no.
int verify(float* a){
    for(int i = 0; i<6; i++){
        if(a[i] != -1.0){
            return 0;
        }
    }
    return 1;
}

int main(){
     Lista * lista = malloc(sizeof(Lista));
     inicializar(lista);
     float a[6];
     float actual = -1.0;
     float n;
     
    char b[100];


     while(read(STDIN_FILENO,a,sizeof(a))>-1 && verify(a)!=1){ 
               lista = agregarNodo(lista, a[0],a[1],a[2],a[3],a[4]);         
     }
     int l = largo(lista);
	__pid_t i = getpid();
	write(STDOUT_FILENO,&l,sizeof(l));
	
	//  close(STDIN_FILENO);
	//  close(STDOUT_FILENO);
	
    return 0;
}