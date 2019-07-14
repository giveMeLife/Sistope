#include "lab3.h"

//***********************************************************************************************
//***********************************************************************************************



/*
Descripción: Función que inicializa una lista
Entrada: Puntero a estructura lista
*/
void inicializar(Llamadas * lista){
	lista->inicio = NULL;
	lista->fin = NULL;
}

void inicializarAscensores(Ascensores * ascensor){
	ascensor->inicio = NULL;
	ascensor->fin = NULL;
}

/*
Descripción: Función que se encarga de agregar un nodo a una lista enlazada
Entrada: Puntero a lista y valores que corresponden a los datos de una visibilidad
Salida: Lista con el nodo agregado
*/
Llamadas*agregarLlamada(Llamadas*lista, int tiempo_llamada, int piso_origen, int piso_destino,int direccion,int cantidad){
	Llamada*llamada;
	llamada=(Llamada*)malloc(sizeof(Llamada));
	llamada->tiempo_llamada = tiempo_llamada;
	llamada->piso_origen = piso_origen;
	llamada->piso_destino = piso_destino;
	llamada->direccion = direccion;
	llamada->cantidad = cantidad;
	llamada->sig=NULL;

	if(lista==NULL){
		lista->inicio=llamada;
		lista->fin = llamada;
	}
	else{
		lista->fin->sig = llamada;
		lista->fin = llamada;
	}
	return lista;
}


Ascensores*agregarAscensor(Ascensores*lista,int capacidad,int direccion,int piso,int pasajeros,int t_subida,int t_bajada,double FS){
	Ascensor*ascensor;
	ascensor=(Ascensor*)malloc(sizeof(Ascensor));
	ascensor->capacidad = capacidad;
	ascensor->direccion = direccion;
	ascensor->piso = piso;
	ascensor->pasajeros = pasajeros;
	ascensor->t_subida = t_subida;
	ascensor->t_bajada = t_bajada;
	ascensor->FS = FS;
	ascensor->estado = 0;
	ascensor->sig=NULL;

	if(lista==NULL){
		lista->inicio=ascensor;
		lista->fin = ascensor;
	}
	else{
		ascensor->sig=lista->inicio;
		lista->inicio=ascensor;
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
	Llamadas * lista = malloc(sizeof(Llamadas));
	Ascensores * ascensor = malloc(sizeof(Ascensores));
	inicializar(lista);
	inicializarAscensores(ascensor);
  //Se abre archivo de texto
	FILE * archivo;
	archivo=fopen(nombre1,"r");
	if(archivo == NULL){
		printf("Error abriendo archivo\n");
		exit(1);
	}

    //Se lee el archivo de texto.
    while (fgets(cadena,500,archivo)!=NULL){ 
			sscanf(cadena,"%d,%d,%d,%s,%d",&tiempo_llamada,&piso_origen,&piso_destino,&direccion,&cantidad);
			printf("Tiempo llamada: %d\nPiso origen: %d\nPiso destino: %d\n Dirección: %c\nCantidad: %d\n", tiempo_llamada,piso_origen,piso_destino,direccion,cantidad );
			if( direccion == d){
            	lista = agregarLlamada(lista, tiempo_llamada, piso_origen, piso_destino, 0, cantidad);
            }
            else{
            	lista = agregarLlamada(lista, tiempo_llamada, piso_origen, piso_destino, 1, cantidad);
            }//inicializar ascensores

            for (int i = 0; i < ascensores; ++i)
            {
            	//agregarAscensor(ascensor*lista,int capacidad,int direccion,int piso,int pasajeros,int t_subida,int t_bajada,double FS)
            	ascensor = agregarAscensor(ascensor,20,1,1,0,0,0,0);
            }
            
		}

		asignarAscensor(lista,ascensor,cantidad,pisos);
	fclose(archivo);

	//Iniciar elevadores con 0 pasajeros y piso 1
	//comprobar parámetros
	

}

int calcularFS(Ascensores*lista,int dir_llamada,int pisos_edificio, int piso_llamada, int piso_destino){
	Ascensor*aux = lista->inicio;
	int FS = 0;
	int cont = 0;
	while(aux!=NULL){
		if(aux->estado == 1 && ((aux->piso <= piso_llamada && aux->direccion == 1)||(aux->piso >= piso_llamada && aux->direccion==0)) && aux->direccion == dir_llamada){
			FS = pisos_edificio + 2 - abs(aux->piso - piso_llamada);
		}
		else if(aux->estado == 1 && ((aux->piso <= piso_llamada && aux->direccion == 1) ||(aux->piso >= piso_llamada && aux->direccion ==0))  && aux->direccion == dir_llamada){
			FS = pisos_edificio + 1 - abs(aux->piso - piso_llamada);
		}
		else if(aux->estado == 1 && aux->direccion == dir_llamada){
			FS = 1;
		}
		else if(aux->estado != 1){
			FS =pisos_edificio + 1 - abs(aux->piso - piso_llamada);
		}
		cont ++;
		aux = aux->sig;
	}
	
	return FS;


}



void asignarAscensor(Llamadas*lista,Ascensores*ascensor, int c_ascensores,int pisos){
	int tiempo = 0;
	Llamada * aux = lista->inicio;
	int FS;
	while(aux!=NULL){
		//actualizar estado
		if(tiempo == aux-> tiempo_llamada){
			FS = calcularFS(ascensor, aux->direccion, pisos,aux->piso_origen, aux->piso_destino);
			printf("%d\n",FS );
			aux = aux->sig;	
		}
		printf("Tiempo: %d\n",tiempo );
		printf("Tiempo llamada: %d\n",aux->tiempo_llamada );
	tiempo = tiempo + 1;
	}
}


