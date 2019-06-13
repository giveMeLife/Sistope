#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <math.h>
#include <ctype.h>


//******************************** VARIABLES GLOBALES ********************************
char buffer[100];
int discos;
float ancho;
int tamBuffer;
int helper;
double **prop;
char * nombreSalida;
int b;



/*
La estructura dato se encarga de almacenar la información ingresada en el archivo de entrada,
los cuales son v y u para el cálculo del radio, la parte real (r), la parte imaginaria (i) y el 
ruido
*/
typedef struct Datos{
	double v;
	double u;
	double r;
	double i;
	double ruido;

}Datos;


/*
La estructura monitor cuenta con:
Dos variables de condición para indicar cuando el buffer de cada hebra se encuentra lleno o noLleno.
El tamaño de cada buffer y la cantidad de datos, los cuales si son comparados permiten saber cuando se llena el buffer.
Un mutex para restringir cuando cada hebra puede escribir en el buffer del proceso principal.
Parciales es un arreglo para almacenar las propiedades parciales de cada hebra
*/
typedef struct Monitor{
	int tamanoBuffer;
	int agregados;
	pthread_cond_t noLleno;
	pthread_cond_t lleno;
	pthread_mutex_t mutex;
	Datos* buffer;
	double * parciales;
}Monitor;




void * consumidor(void * monitor);
void writeFile(char * nombreArchivo, int b);
double raiz(double a, double b);
int disco(double a, double b);
void readFile(char* name, char * nombreSalida, int b, int tamanoBuffer);
void agregarMonitor(Monitor *m ,double* data);
void vaciar(Monitor *m);


