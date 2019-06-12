#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <math.h>
#include <ctype.h>


#define LECTURA 0
#define ESCRITURA 1


//Estructura nodo que incluye todos los datos de una visibilidad 
typedef struct Nodo{
	double v;
	double u;
	double r;
	double i;
	double ruido;
	struct Nodo*sig;

}Nodo;


typedef struct Datos{
	double v;
	double u;
	double r;
	double i;
	double ruido;

}Datos;

//Estructura que contiene el primer y último elemento de una lista enlazada
typedef struct Lista{
	Nodo * inicio;
	Nodo * fin;

	
}Lista;

typedef struct Monitor{
	int tamanoBuffer;
	int agregados;
	pthread_cond_t noLleno;
	pthread_cond_t lleno;
	pthread_mutex_t mutex;
	Datos* buffer;
}Monitor;

typedef struct Parametros{
	double * data;
	Monitor * monitores;

}Parametros;


void * prueba(void * a);

void writeFile(char * nombreArchivo, int b);
double raiz(double a, double b);
int disco(double a, double b);
void readFile(char* name, char * nombreSalida, int b, int tamanoBuffer);
void inicializar(Lista * lista);
Lista* agregarNodo(Lista*lista, double u, double v, double r, double i, double ruido);
double propiedades(Lista* lista, int tipo, int N);
Lista*agregarNodo(Lista*lista, double u, double v, double r, double i, double ruido);
