#include "lab3.h"

//***********************************************************************************************
//***********************************************************************************************




/*
Descripción: Función que se encarga de agregar un nodo a una lista enlazada
Entrada: Puntero a lista y valores que corresponden a los datos de una visibilidad
Salida: Lista con el nodo agregado
*/
Llamada*agregarLlamada(Llamada*lista, int tiempo_llamada, int piso_origen, int piso_destino,int direccion,int cantidad){
	Llamada*llamada;
	llamada=(Llamada*)malloc(sizeof(Llamada));
	llamada->tiempo_llamada = tiempo_llamada;
	llamada->piso_origen = piso_origen;
	llamada->piso_destino = piso_destino;
	llamada->direccion = direccion;
	llamada->cantidad = cantidad;
	llamada->sig=NULL;

	if(lista==NULL){
		lista=llamada;
	}
	else{
		Llamada *aux = lista;
		while(aux->sig!=NULL){
			aux = aux->sig;
		}
		aux->sig = llamada;
	}
	return lista;
}


Proxima_Llamada*nuevaLlamada(Proxima_Llamada*lista,int direccion,int cantidad_pasajeros,int destino){
	Proxima_Llamada*nuevo;
	nuevo=(Proxima_Llamada*)malloc(sizeof(Proxima_Llamada));
	nuevo->direccion = direccion;
	nuevo->cantidad_pasajeros = cantidad_pasajeros;
	nuevo->destino =destino;
	nuevo->sig=NULL;

	if(lista==NULL){
		lista=nuevo;
	}

	else{
		Proxima_Llamada *aux = lista;
		while(aux->sig!=NULL){
			aux = aux->sig;
		}

		aux->sig = nuevo;
	}
	return lista;
}


/*
Entrada: Puntero a char (nombre archivo de salida), entero (indica si se escriben las visibilidades por hijo)
Descripción: Se recorre el arreglo que contiene las propiedades calculadas por cada hebra y se escriben en 
			 el archivo de salida. Si el entero b es 1 se muestran las visibilidades.
Salida: archivo con los resultados.

*/
void writeFile(char * nombreArchivo, int b){
	FILE *archivoSalida=fopen(nombreArchivo,"w");
	int i = 0;
	/*
	while(i < discos){
		fprintf(archivoSalida, "Disco %d:\n",i+1);
		fprintf(archivoSalida, "Media real: %f\n",prop[i][0]);
		fprintf(archivoSalida, "Media imaginaria: %f\n",prop[i][1]);
		fprintf(archivoSalida, "Potencia: %f\n",prop[i][2]);
		fprintf(archivoSalida, "Ruido total: %f\n",prop[i][3]);
		if(b == 1){
			printf("Soy la hebra %i y procesé %i visibilidades\n",i+1,(int)prop[i][4]);
		}
    fprintf(archivoSalida,"\n");
		i++;
	}	
	*/
	fclose(archivoSalida);
}

void print(Llamada* l){
	Llamada* aux = l;
	int i = 0;
	while(aux->sig !=NULL){
		
		printf("%d\n",i);
		i = i+1;
		aux = aux->sig;
	}
}

void getAcensor(Ascensor* l, int index){
	int i = 0;

}
//***********************************************************************************************
//***********************************************************************************************



void readFile(char* nombre1, char * nombre2, char * nombre3, int pisos, int ascensores, int bvalue){
	char cadena[500];
	int opcion;
	int tiempo_llamada;
	int piso_origen;
	int piso_destino;
	char direccion;
	int cantidad;
	int validador = 0;
	int d = 'D';
	Llamada * lista = malloc(sizeof(Llamada));
	Ascensor * ascensor = (Ascensor *)malloc(ascensores* sizeof(Ascensor));
  //Se abre archivo de texto
	FILE * archivo;
	archivo=fopen(nombre1,"r");
	if(archivo == NULL){
		printf("Error abriendo archivo\n");
		exit(1);
	}

    //Se lee el archivo de texto.
    while (fgets(cadena,500,archivo)!=NULL){ 
			sscanf(cadena,"%d,%d,%d,%c,%d",&tiempo_llamada,&piso_origen,&piso_destino,&direccion,&cantidad);
		//	printf("cadena: %s\n",cadena );
		//	printf("Tiempo llamada: %d\nPiso origen: %d\nPiso destino: %d\n Dirección: %c\nCantidad: %d\n", tiempo_llamada,piso_origen,piso_destino,direccion,cantidad );
			if( direccion == d){
            	lista = agregarLlamada(lista, tiempo_llamada, piso_origen, piso_destino, 0, cantidad);
            }
            else{
            	lista = agregarLlamada(lista, tiempo_llamada, piso_origen, piso_destino, 1, cantidad);
            }//inicializar ascensores

            for (int i = 0; i < ascensores; ++i)
            {
            	//agregarAscensor(ascensor*lista,int capacidad,int direccion,int piso,int pasajeros,int t_subida,int t_bajada,double FS)
            	//ascensor = agregarAscensor(ascensor,20,1,1,0,0,0,0);
            	ascensor[i].capacidad = 20;
							ascensor[i].direccion = direccion;
							ascensor[i].piso_actual = 1;
							ascensor[i].piso_destino = 1;
							ascensor[i].pasajeros = 0;
							ascensor[i].t_subida = 0;
							ascensor[i].t_bajada = 0;
							ascensor[i].descargando = 0;
							ascensor[i].puertas = 0;
							ascensor[i].estado = 0;
							ascensor[i].cola = NULL;
            }
            
		}
		printf("hola\n");
		print(lista);
		asignarAscensor(lista,ascensor,cantidad,pisos);
		fclose(archivo);

	//Iniciar elevadores con 0 pasajeros y piso 1
	//comprobar parámetros
	

}

Ascensor* calcularFS(Ascensor*lista,int dir_llamada,int pisos_edificio, int piso_llamada, int piso_destino,int c_ascensores,int c_pasajeros){
	int FS = 0;
	int cont = 0;
	int * resultado[2];
	int i = 0;
	int max = 0;
	int asc = 0;
	while(i< c_ascensores){
		if(lista[i].estado == 1 && ((lista[i].piso_actual <= piso_llamada && lista[i].direccion == 1)||(lista[i].piso_actual >= piso_llamada && lista[i].direccion==0)) && lista[i].direccion == dir_llamada){
			FS = pisos_edificio + 2 - abs(lista[i].piso_actual - piso_llamada);
		}
		else if(lista[i].estado == 1 && ((lista[i].piso_actual <= piso_llamada && lista[i].direccion == 1) ||(lista[i].piso_actual >= piso_llamada && lista[i].direccion ==0))  && lista[i].direccion == dir_llamada){
			FS = pisos_edificio + 1 - abs(lista[i].piso_actual - piso_llamada);
		}
		else if(lista[i].estado == 1 && lista[i].direccion == dir_llamada){
			FS = 1;
		}
		else if(lista[i].estado != 1){
			FS =pisos_edificio + 1 - abs(lista[i].piso_actual - piso_llamada);
		}
		i++;
		if(FS > max){
			printf("FS: %d\n", FS);
			max = FS;
			asc = i;
		}
		lista[max-1].cola = nuevaLlamada(lista[max-1].cola, dir_llamada,c_pasajeros, piso_destino);
	}
	/*
	lista[max].cola->direccion = dir_llamada;
	lista[max].cola->cantidad_pasajeros = c_pasajeros;
	lista[max].cola->destino = piso_destino;
	*/
	return lista;
}


Ascensor * actualizar(Ascensor * ascensor){
	/*
	Ascensor * aux  = ascensor;
	while(aux!=NULL){
		//Si está descargando personas 
		if(aux->descargando == 1 && aux->pasajeros>0){
			aux->pasajeros = aux->pasajeros-1;
			//abrir puertas
			//sacar de la cola
			//asignar nuevo destino de ser necesario
		}
		//Si el piso al que llega es igual al de destino y las puertas están cerradas
		else if(aux->piso_actual == aux->piso_destino && aux->puertas==0){
			aux->puertas = 1;
		}
		//Si el piso al que llega es igual al de destino y las puertas están abiertas y ya no quedan
		//pasajeros, se cierran las puertas. AÑADIR PASAJEROS DEL DESTINO == 0
		else if(aux->piso_actual == aux->piso_destino && aux->puertas==1){
			aux->puertas = 0;
			aux->descargando = 0;
		}
	//	else if(aux->piso_actual == aux->piso_destino && aux->personas)




		aux = aux->sig;
	}
	*/

}

void asignarAscensor(Llamada*lista,Ascensor*ascensor, int c_ascensores,int pisos){
	
	int tiempo = 0;
	Llamada * aux = lista;
	int FS=0;
	while(aux!=NULL){
		//actualizar estado
	//cl	printf("Tiempo llamada: %d\n",aux->tiempo_llamada );
		if(tiempo == aux-> tiempo_llamada){
		//	ascensor = actualizar();
			printf("hi\n");
			ascensor = calcularFS(ascensor, aux->direccion, pisos,aux->piso_origen, aux->piso_destino,c_ascensores,aux->cantidad);
		//	printf("FS: %d\n",FS );
		//	printf("Tiempo: %d\n",tiempo );
			printf("Tiempo llamada: %d\n",aux->tiempo_llamada );
			
				
		}
		aux = aux->sig;
		
	tiempo = tiempo + 1;
	}
	
}


