#include "lab3.h"

//***********************************************************************************************
//***********************************************************************************************

/*
Entrada: lista con las próximas llamadas que debe atender el ascensor
Descripción: Se calcula el largo que tiene la lista, es decir, la cantidad de llamadas
Salida: Cantidad de llamadas de un ascensor
*/
int largoL(Proxima_Llamada* l){
	int i = 0;
	Proxima_Llamada* aux = l;
	while(aux->sig != NULL){
		aux = aux->sig;
		i++;
	}
	return i;
}


//***********************************************************************************************
//***********************************************************************************************

/*
Entrada: lista con las próximas llamadas que debe atender el ascensor y el índice respectivo
Descripción: Se elimina el elemento de la lista en base al índice ingresado
Salida: lista
*/

Proxima_Llamada* eliminarLlamada(Proxima_Llamada* l, int index){
	int i = 1;
	int largo = largoL(l);
	Proxima_Llamada* aux = l;
	if(index == 0){
		Proxima_Llamada* siguiente = l->sig;
		free(l);
		return siguiente;
	}
	else if(index >= largo){
		return NULL;
	}
	while(aux->sig!=NULL){
		if(i == index){
			Proxima_Llamada* s = aux->sig;
			aux->sig = s->sig;
			free(s);
			return l; 
		}
		i++;
		aux = aux->sig;
	}
}


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


//***********************************************************************************************
//***********************************************************************************************

/*
Descripción: Función que se encarga de agregar un nodo a una lista enlazada
Entrada: Puntero a lista y valores que corresponden a los datos de una visibilidad
Salida: Lista con el nodo agregado
*/
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

//***********************************************************************************************
//***********************************************************************************************

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



//***********************************************************************************************
//***********************************************************************************************
/*
Entrada: nombre de los archivos de entrada, cantidad de pisos del ascensor, ascensores y validador
Descripción: Se recorre el archivo de entrada y se añade a una lista de llamadas. También se crean los ascensores
			 junto con sus respectivos valores iniciales. Se llama a la función asignar ascensor.
Salida:
*/


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
	Llamada * lista = (Llamada*)malloc(sizeof(Llamada));
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
            	lista = agregarLlamada(lista, tiempo_llamada, piso_origen, piso_destino, 68, cantidad);
            }
            else{
            	lista = agregarLlamada(lista, tiempo_llamada, piso_origen, piso_destino, 85, cantidad);
            }//inicializar ascensores
       //     printf("direccion: %d\n",direccion );

            for (int i = 0; i < ascensores; ++i)
            {
            	//agregarAscensor(ascensor*lista,int capacidad,int direccion,int piso,int pasajeros,int t_subida,int t_bajada,double FS)
            	//ascensor = agregarAscensor(ascensor,20,1,1,0,0,0,0);
            	ascensor[i].capacidad = 20;
							ascensor[i].direccion = 85;
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
		
		//print(lista);
		asignarAscensor(lista,ascensor,cantidad,pisos);
		fclose(archivo);

	//Iniciar elevadores con 0 pasajeros y piso 1
	//comprobar parámetros
	

}


//***********************************************************************************************
//***********************************************************************************************

/*
Entrada: Lista de ascensores, dirección, piso y destino de la nueva llamada, pisos del edificio y cantidad de ascensores
Descripción: Se calcula el valor FS para cada uno de los ascensores,luego al máximo se le asigna la nueva llamada.
Salida: Se retorna la lista de ascensores.
*/
Ascensor* calcularFS(Ascensor*lista,int dir_llamada,int pisos_edificio, int piso_llamada, int piso_destino,int c_ascensores,int c_pasajeros){
	int FS = 0;
	int cont = 0;
	int * resultado[2];
	int i = 0;
	int max = 0;
	int asc = 0;
	while(i< c_ascensores){
		if(lista[i].pasajeros+c_pasajeros>=20 && lista[i].estado == 1 && ((lista[i].piso_actual <= piso_llamada && lista[i].direccion == 1)||(lista[i].piso_actual >= piso_llamada && lista[i].direccion==0)) && lista[i].direccion == dir_llamada){
			FS = pisos_edificio + 2 - abs(lista[i].piso_actual - piso_llamada);
		}
		else if(lista[i].pasajeros+c_pasajeros>=20 && lista[i].estado == 1 && ((lista[i].piso_actual <= piso_llamada && lista[i].direccion == 1) ||(lista[i].piso_actual >= piso_llamada && lista[i].direccion ==0))  && lista[i].direccion == dir_llamada){
			FS = pisos_edificio + 1 - abs(lista[i].piso_actual - piso_llamada);
		}
		else if(lista[i].pasajeros + c_pasajeros>=20 && lista[i].estado == 1 && lista[i].direccion == dir_llamada){
			FS = 1;
		}
		else if(lista[i].estado == 1){
			FS =pisos_edificio + 1 - abs(lista[i].piso_actual - piso_llamada);
		}
		i++;
		if(FS > max){
		//	printf("FS: %d\n", FS);
			max = FS;
			asc = i;
		}
		
	}
	//printf("Ascensor asignado: %d\n",asc+1 );
	if(asc !=0){
		asc--;
		}
	lista[asc].cola = nuevaLlamada(lista[asc].cola, dir_llamada,c_pasajeros, piso_destino);

		
	//Si el ascensor no tiene llamadas asignadas y se encuentra en el piso de la llamada, se asigna el piso de destino
	if(lista[asc].estado ==0 || lista[asc].estado ==2){
		if(lista[asc].piso_actual == piso_llamada){
		//	printf("1\n");
			lista[asc].piso_destino = piso_destino;
			lista[asc].pasajeros = lista[asc].pasajeros + c_pasajeros;
			lista[asc].estado = 1;
			lista[asc].cola = eliminarLlamada(lista[asc].cola,0);

		}
		else{
		//	printf("2\n");
			lista[asc].piso_destino = piso_llamada;
			lista[asc].estado =2;

		}



	}
	return lista;
}


//***********************************************************************************************
//***********************************************************************************************

/*
Entrada: Lista con los ascensores, su cantidad y los pisos del edificio
Descripción: En base a los valores que tiene cada ascensor se actualiza su estados. Se pueden descargar pasajeros
			 cargar, abrir y cerrar puertas, bajar o subir. Solo puede suceder una acción en 1 minuto.
Salida: Lista de ascensores actualizada.
*/
Ascensor * actualizar(Ascensor * ascensor, int c_ascensores, int pisos){
	int i = 0;
//	printf("Pisos: %d\n",pisos );
	while(i < c_ascensores){
	//	printf("Destino: %d\n", ascensor[i].piso_destino);
	//	printf("Estado: %d\nDirección: %d\nPiso actual: %d\n",ascensor[i].estado,ascensor[i].direccion,ascensor[i].piso_actual);
		//Si está descargando personas 
		if(ascensor[i].descargando == 1 && ascensor[i].pasajeros>0){
			ascensor[i].pasajeros = ascensor[i].pasajeros-1;
			ascensor[i].cola->cantidad_pasajeros = ascensor[i].cola->cantidad_pasajeros-1;
			//printf("Ascensor: %d - Descargando\n",i+1);
			//printf("Actual: %d - Destino: %d\n",ascensor[i].piso_actual, ascensor[i].piso_destino );

			//abrir puertas
			//sacar de la cola
			//asignar nuevo destino de ser necesario
		}
		//Si el piso al que llega es igual al de destino y las puertas están cerradas
		else if(ascensor[i].piso_actual == ascensor[i].piso_destino && ascensor[i].puertas==0 && ascensor[i].pasajeros>0){
			ascensor[i].puertas = 1;
			ascensor[i].descargando = 1;
			//printf("Ascensor: %d - Se abren las puertas\n",i+1);
			//printf("Actual: %d - Destino: %d\n",ascensor[i].piso_actual, ascensor[i].piso_destino );

		}
		//Si el piso al que llega es igual al de destino y las puertas están abiertas y ya no quedan
		//pasajeros de ningún piso, se cierran las puertas. AÑADIR PASAJEROS DEL DESTINO == 0
		else if(ascensor[i].piso_actual == ascensor[i].piso_destino && ascensor[i].puertas==1 && ascensor[i].pasajeros==0){
			ascensor[i].puertas = 0;
			ascensor[i].descargando = 0;
			ascensor[i].estado = 0;
			//printf("Ascensor: %d - Se cierran las puertas\n",i+1);
			//printf("Actual: %d - Destino: %d\n",ascensor[i].piso_actual, ascensor[i].piso_destino );

		}
		else if(ascensor[i].estado == 1 && ascensor[i].direccion == 68 && ascensor[i].piso_actual>1){
			ascensor[i].piso_actual = ascensor[i].piso_actual -1;
			//printf("Ascensor: %d - bajando\n",i+1);
			//printf("Actual: %d - Destino: %d\n",ascensor[i].piso_actual, ascensor[i].piso_destino );
		}
		else if(ascensor[i].estado == 1 && ascensor[i].direccion == 85 && ascensor[i].piso_actual<pisos){
			ascensor[i].piso_actual = ascensor[i].piso_actual +1;
			//printf("Ascensor: %d - Subiendo\n", i +1);
			//printf("Actual: %d - Destino: %d\n",ascensor[i].piso_actual, ascensor[i].piso_destino );

		}
		else if(ascensor[i].estado == 2 && ascensor[i].direccion == 85 && ascensor[i].piso_actual>ascensor[i].piso_destino){
			ascensor[i].piso_actual = ascensor[i].piso_actual -1;
			//printf("Ascensor: %d - bajando\n",i+1);
			//printf("Actual: %d - Destino: %d\n",ascensor[i].piso_actual, ascensor[i].piso_destino );

		}
		else if(ascensor[i].estado == 2 && ascensor[i].direccion == 68 && ascensor[i].piso_actual<ascensor[i].piso_destino){
			ascensor[i].piso_actual = ascensor[i].piso_actual -1;
			//printf("Ascensor: %d - bajando\n",i+1);
			//printf("Actual: %d - Destino: %d\n",ascensor[i].piso_actual, ascensor[i].piso_destino );

		}
		/*
		else if(ascensor[i].estado == 2 && ascensor[i].piso_actual==ascensor[i].piso_destino){
			ascensor[asc].piso_destino = piso_destino;
			lista[asc].pasajeros = lista[asc].pasajeros + c_pasajeros;
			lista[asc].estado = 1;
			lista[asc].cola = eliminarLlamada(lista[asc].cola,0);
		}
		*/
	//	printf("Piso actual: %d\n",ascensor[i].piso_actual );



		i++;
	}
	return ascensor;
	

}


//***********************************************************************************************
//***********************************************************************************************
/*
Entrada: Lista con las llamadas del ascensor, ascensor, cantidad de ascensores y número de pisos
Descripción: La función encuentra una coincidencia entre el tiempo actual y el tiempo de la llamada,
			 de ser así se calcula el FS y se asigna la llamada, en caso contrario se actualizan
			 los valores del ascensor y se aumenta el tiempo en 1 segundo.
Salida: Se llama al archivo de escritura.
*/

void asignarAscensor(Llamada*lista,Ascensor*ascensor, int c_ascensores,int pisos){
	
	int tiempo = 0;
	Llamada * aux = lista;
	int FS=0;
	while(aux!=NULL){
		//actualizar estado
		//printf("Tiempo llamada: %d\n",aux->tiempo_llamada );
		if(tiempo == aux-> tiempo_llamada){	
			ascensor = calcularFS(ascensor, aux->direccion, pisos,aux->piso_origen, aux->piso_destino,c_ascensores,aux->cantidad);
		//	printf("FS: %d\n",FS );
		//	printf("Tiempo: %d\n",tiempo );
		//	printf("Tiempo llamada: %d\n",aux->tiempo_llamada );
			
		aux = aux->sig;

		}
		ascensor = actualizar(ascensor,c_ascensores,pisos);
		tiempo = tiempo + 1;
	}
	
}


