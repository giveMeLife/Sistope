#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <math.h>
#include <ctype.h>


//******************************** VARIABLES GLOBALES ********************************



//Estructura nodo que incluye todos los datos de una llamada de ascensor 
typedef struct Llamada{
	int tiempo_llamada;
	int piso_origen;
	int piso_destino;
	int direccion;
	int cantidad;
	struct Llamada*sig;

}Llamada;


typedef struct Proxima_Llamada{
	int direccion;
	int cantidad_pasajeros;
	int destino;
	struct Proxima_Llamada*sig;
}Proxima_Llamada;


/*
La estructura ascensor contiene la dirección, capacidad, el piso actual, los pasajeros que lleva, el 
tiempo de subida total y el tiempo de bajada.

*/
typedef struct Ascensor{
	int capacidad;
	int direccion;
	int piso_actual;
	int piso_destino;
	int pasajeros;
	int t_subida;
	int t_bajada;
	int estado;
	int descargando;
	int puertas;
	Proxima_Llamada*cola;
}Ascensor;



void readFile(char* nombre1, char * nombre2, char * nombre3, int pisos, int ascensores, int bvalue);
Llamada*agregarLlamada(Llamada*lista, int tiempo_llamada, int piso_origen, int piso_destino,int direccion,int cantidad);
void asignarAscensor(Llamada*lista,Ascensor*ascensor, int c_ascensores,int pisos);
