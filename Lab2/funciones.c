#include "lab2.h"


/*
Entrada: Coordenadas a y b del tipo double.
Descripción: Función que calcula la distancia al origen de un punto en el plano
Salida: Retorna la suma de los cuadrados de las variables ingresadas
*/
double raiz(double a, double b){
	return sqrt(a*a + b*b);
}

//***********************************************************************************************
//***********************************************************************************************


/*
Entrada: Puntos representado como dos coordenadas double
Descripción: La función permite calcular a qué disco corresponde cada coordenada
Salida: Un valor int que indica al disco al que pertenece el punto.
*/

int disco(double a, double b){
    double punto = raiz(a,b);
    double disc = punto/(double)ancho;
    if(punto>=(double)(discos*ancho)){
        return (int)discos-1;
    }
    return (int)disc;

}

//***********************************************************************************************
//***********************************************************************************************



/*
Entrada: Monitor m con sus parámetros asociados y arreglo data que contiene los valores del archivo
		 de entrada (u,v,r,i y el ruido).
Descripción: AgregarMonitor añade los valores del archivo de entrada a cada una de las hebras. Primero bloquea el mutex,
			 ya que si no se ha producido no se puede consumir, luego  comprueba si el buffer está lleno, si lo está 
			 envía un signal para que se puedan calcular las propiedades parciales. Además envía un wait a noLleno.
			 Si el buffer no está lleno se agregan las variables que contiene data.
Salida: - 

*/
void agregarMonitor(Monitor *m ,double* data){
	pthread_mutex_lock(&m->mutex);
	while(m->agregados == m->tamanoBuffer){
		pthread_cond_signal(&m->lleno);
		pthread_cond_wait(&m->noLleno,&m->mutex);
	}
		m->buffer[m->agregados].u = data[0] +0.0;
		m->buffer[m->agregados].v = data[1] +0.0;
		m->buffer[m->agregados].r = data[2] +0.0;
		m->buffer[m->agregados].i = data[3] +0.0;
		m->buffer[m->agregados].ruido = data[4]+ 0.0;
		m->agregados++;

	pthread_mutex_unlock(&m->mutex);;

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
	fclose(archivoSalida);
}


//***********************************************************************************************
//***********************************************************************************************



/*
Entrada: Puntero a char (nombre archivo de texto), puntero a char (nombre archivo de texto de salida), entero que indica si se escriben visibildades por hijo o no
Descripción: La función lee el archivo ingresado, crea las hebras y monitores en relación a la cantidad de discos y las envía a la función consumidor,
			 les asigna las variables iniciales y asigna la memoria a los valores que lo necesitan. Luego lee el archivo, agrega los datos a la estructura monitor
			 para que las hebras los puedan utilizar. Cuando termina de leer el archivo, realiza signal para que en la función consumidor las hebras puedan procesar sus
			 propiedades.
Salida:  Hebras procesadas.
*/
void readFile(char* name, char * nombreSalida, int b, int tamBuffer){
	char cadena[500];
	int opcion;
	double v;
	double u;
	double r;
	double i;
	double ruido;
  double data[5];
	
  //Se abre archivo de texto
	FILE * archivo;
	archivo=fopen(name,"r");
	if(archivo == NULL){
		printf("Error abriendo archivo\n");
		exit(1);
	}
	int j = 0;
    void * a = 0;
    pthread_t hebras[discos];
    monitores = (Monitor *)malloc(discos* sizeof(Monitor));
    //Se inicializan los monitores
    //Se crean las hebras en base al número de discos y se le asigna la función procesar
    for (int i = 0; i < discos; ++i)
    {
    	pthread_cond_init(&monitores[i].noLleno,NULL);
		pthread_cond_init(&monitores[i].lleno,NULL);
		pthread_mutex_init(&monitores[i].mutex,NULL);
    	monitores[i].tamanoBuffer = tamBuffer;
		monitores[i].agregados = 0;
		monitores[i].buffer = (Datos*)malloc(sizeof(Datos)*tamBuffer);
		monitores[i].parciales = (double*)malloc(sizeof(double)*5);
		pthread_create(&hebras[i],NULL,consumidor, &monitores[i]);
    }

    //Se lee el archivo de texto y se envían los datos a consumidor.
    while (fgets(cadena,500,archivo)!=NULL){ 
			sscanf(cadena,"%lf,%lf,%lf,%lf,%lf",&u,&v,&r,&i,&ruido);
            data[0] = u;
            data[1] = v;
            data[2] = r;
            data[3] = i;
            data[4] = ruido;
            int disc = disco(v,u);
			agregarMonitor(&monitores[disc],data);
		}
		helper = 0;
	//Si se termina de leer el archivo se envía signal a todas las hebras para que puedan calcular
	//sus propiedades
		for(j = 0; j<discos;j++){

			pthread_mutex_unlock(&monitores[j].mutex);
			pthread_cond_signal(&monitores[j].lleno);
			pthread_cond_signal(&monitores[j].noLleno);
		}


		

    j = 0;
    //Para que las hebras no mueran antes de terminar se utiliza join y se agregan a la estructura común
    while(j < discos){
			printf("%d\n",j);
    	pthread_join(hebras[j],NULL);
    // 	if((monitores[j].parciales[4]) > 0){
    // 	prop[j][0] = (monitores[j].parciales[0])/(monitores[j].parciales[4]);
		// prop[j][1] = (monitores[j].parciales[1])/monitores[j].parciales[4];
		// prop[j][2] = (monitores[j].parciales[2]);
		// prop[j][3] = (monitores[j].parciales[3]);
		// prop[j][4] = monitores[j].parciales[4];
		// }
		// else{
		// 	prop[j][0] = 0.0;
		// 	prop[j][1] = 0.0;
		// 	prop[j][2] = 0.0;
		// 	prop[j][3] = 0.0;
		// }
		j++;

  }
	j=0;
	while(j<discos){
			printf("Valor: %d\n",j);
			int a = j+0;
			pthread_create(&hebras[a],NULL, calculoProp,(void *) &a);
			pthread_join(hebras[j], NULL);
    	j++;
	}
	// while(j < discos){
  //   	pthread_join(hebras[j],NULL);
  //   	j++;
  // }
}


//***********************************************************************************************
//***********************************************************************************************


/*
Entrada: Monitor con sus parámetros
Descripción: La función se encarga de sacar los parámetros del buffer cuando se encuentra llego y 
			 calcular las propiedades parciales (media real, imaginaria, potencia y ruido). Luego deja 
			 a cada elemento del buffer con valor 0  y también a la cantidad de datos que este posee.
Salida: Buffer vacío (con valores 0) y propiedades parciales.
*/

void vaciar(Monitor *m){
	double mediaR = 0.0;
	double mediaI = 0.0;
	double potencia = 0.0;
	double ruido = 0.0;

	for(int i = 0; i<m->agregados;i++){
		mediaR = m->buffer[i].r + mediaR;
		mediaI = m->buffer[i].i + mediaI;
		potencia = sqrt(pow(m->buffer[i].r,2) + pow(m->buffer[i].i,2)) + potencia;
		ruido = m->buffer[i].ruido + ruido;
		
		m->buffer[i].u = 0.0;
		m->buffer[i].v = 0.0;
		m->buffer[i].r = 0.0;
		m->buffer[i].i = 0.0;
		m->buffer[i].ruido = 0.0;
	}
	m->parciales[0] = m->parciales[0] + mediaR;
	m->parciales[1] = m->parciales[1] + mediaI;
	m->parciales[2] = m->parciales[2] + potencia;
	m->parciales[3] = m->parciales[3] + ruido;
	m->parciales[4] = m->parciales[4] + m->agregados; 

	

	m->agregados = 0;


}

//***********************************************************************************************
//***********************************************************************************************

/*
Entrada: Monitor
Descripción: La función se encarga de vaciar el buffer cuando este se encuentre lleno, por lo cual
			 en primera instancia bloquea el mutex, luego pregunt si la cantidad de agregados es igual
			 a 0, si lo es, se bloquea el monitor ya que no está lleno. Si al desbloquearse sigue sin estar lleno
			 significa que la hebra no procesó ningún valor y se finaliza.
			 Debido a que esta funcións se llama al crear la hebra, siempre se ingresa al while. Si la hebra se desbloquea
			 porque está llena se llama a vaciar para calcular las propiedades parciales.
			 y se desbloquea el monitor no lleno,ya que se vació el buffer y el mutex.
Salida: Buffer vacío
*/

void * consumidor(void * monitor){

	while(helper == 1){
		Monitor *p;
		p = (Monitor *) monitor;
		pthread_mutex_lock(&p->mutex);
		while(p->agregados == 0){
			pthread_cond_wait(&p->lleno,&p->mutex);
			if(p->agregados == 0)
				pthread_exit(NULL);
		}
		vaciar(p);
		pthread_cond_signal(&p->noLleno);
		pthread_mutex_unlock(&p->mutex);
	}
}

void * calculoProp(void * j){
	int aux; 

	aux = *((int*)j) ;
	printf("Aux: %d\n", aux);
	if(monitores[aux].parciales[4] > 0){
    prop[aux][0] = (monitores[aux].parciales[0])/(monitores[aux].parciales[4]);
		prop[aux][1] = (monitores[aux].parciales[1])/monitores[aux].parciales[4];
		prop[aux][2] = (monitores[aux].parciales[2]);
		prop[aux][3] = (monitores[aux].parciales[3]);
		prop[aux][4] = monitores[aux].parciales[4];
	}
	else{
			prop[aux][0] = 0.0;
			prop[aux][1] = 0.0;
			prop[aux][2] = 0.0;
			prop[aux][3] = 0.0;
	}
	pthread_exit(NULL);

}

