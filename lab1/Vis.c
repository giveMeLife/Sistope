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
	double v;
	double u;
	double r;
	double i;
	double ruido;
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


Lista*agregarNodo(Lista*lista, double u, double v, double r, double i, double ruido){
	Nodo*nodo;
	nodo=(Nodo*)malloc(sizeof(Nodo));
	nodo->v=v;
	nodo->u=u;
	nodo->r=r;
	nodo->i=i;
	nodo->ruido=ruido;

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
double propiedades(Lista* lista, int tipo, int N){
	if(N==0 && (tipo == 0 || tipo == 1)){
		return (double)0.0;
	}
	double pot = 0.0;
	double sumR = 0.0;
	double sumI = 0.0;
	double sumRuido = 0.0;
	
	Nodo * actual = lista->inicio;

	while(actual!=NULL){
		sumR = sumR + actual->r;
		sumI = sumI + actual->i;		
		sumRuido = sumRuido + actual->ruido;
		pot = pot + sqrt(pow(actual->r,2) + pow(actual->i,2));
		actual = actual->sig;


	}
	

	//Media Real
	if(tipo == 0){
		return sumR/(double)N;}
	//Media imaginaria
	else if(tipo == 1){
		return sumI/(double)N;}
	//Potencia
	else if(tipo == 2){
		return pot;}
	else{
		return sumRuido;
	}
}

//Verifica si el padre dejará de mandar datos o no.
int verify(double* a){
    for(int i = 0; i<5; i++){
        if(a[i] != -1.0){
            return 0;
        }
    }
    return 1;
}

int main(){
	//Se inicializa lista para guardar los datos
    Lista * lista = malloc(sizeof(Lista));
    inicializar(lista);

	//arreglo para la lectura desde el proceso padre
    double a[5];
	//arreglo para las propiedades
    double *prop = malloc(sizeof(double)*5);
	
	//Visibilidades;
	prop[4] = 0.0;
    
	//Lectura del PIPE 
	while(read(STDIN_FILENO,a,sizeof(a))>-1 && verify(a)!=1){ 
        lista = agregarNodo(lista, a[0],a[1],a[2],a[3],a[4]);         
		prop[4] = prop[4] + 1.0;
	}

    int l = largo(lista);
	prop[0] = propiedades(lista,0,l);
	prop[1] = propiedades(lista,1,l);
	prop[2] = propiedades(lista,2,l);
	prop[3] = propiedades(lista,3,l); 

	//Envío de mensaje con datos al padre
	write(STDOUT_FILENO, prop, sizeof(double)*5);
	
	// freopen ("/dev/tty", "a", stdout);
	// if(l>0)
	// 	printf("Data:%lf\n",prop[2]);
    // return 0;
}