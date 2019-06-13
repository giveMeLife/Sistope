#include "lab2.h"

char buffer[100];
int discos;
float ancho;
int tamBuffer;
double **prop;
int helper;
Lista * lista;
double **prop;
char * nombreSalida;
int b;
/*
Descripción: Función que inicializa una lista
Entrada: Puntero a estructura lista
Salida: 
*/
void inicializar(Lista * lista){
	lista->inicio = NULL;
	lista->fin = NULL;
}
/*
Descripción: Función que se encarga de agregar un nodo a una lista enlazada
Entrada: Puntero a lista y valores que corresponden a los datos de una visibilidad
Salida: Lista con el nodo agregado
*/
Lista*agregarNodo(Lista*lista, double u, double v, double r, double i, double ruido){
	Nodo*nodo;
	nodo=(Nodo*)malloc(sizeof(Nodo));
	nodo->mediaR=v;
	nodo->mediaI=u;
	nodo->potencia=r;
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






/*
Descripción: Función que calcula la distancia al origen de un punto en el plano
Entrada: Punto representados como dos coordenadas double.
Salida: La raíz del cuadrado de a sumado con el cuadrado de b
*/
double raiz(double a, double b){
	return sqrt(a*a + b*b);
}

/*
Descripción: Función que indica en qué disco se encuentra un punto
Entrada: Puntos representado como dos coordenadas double
Salida: Un valor int que indica al disco al que pertenece el punto
*/
int disco(double a, double b){
    double punto = raiz(a,b);
    double disc = punto/(double)ancho;
    if(punto>=(double)(discos*ancho)){
        return (int)discos-1;
    }
    return (int)disc;

}


//Productor con consumidor tipo C
void agregar(Monitor *m ,double* data){
	printf("agregar: %lf\n", data[2]);
	printf("Cantidad: %d\n", m->agregados);
	pthread_mutex_lock(&m->mutex);
	while(m->agregados == m->tamanoBuffer){
		printf("me llené\n");
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





//Se escribe el archivo de salida según el nombre ingresado por el usuario, la lista con las propiedades
// y el archivo de salida.
/*
Entrada: Puntero a char (nombre archivo de salida), entero (indica si se escriben las visibilidades por hijo)

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
		}
    fprintf(archivoSalida,"\n");
		i++;
	}	
	fclose(archivoSalida);
}






//función que lee el archivo de texto y se encarga de 
//mandar los datos a los hijos y recibir las respuestas
//de estos.
/*
Entrada: Puntero a char (nombre archivo de texto), puntero a char (nombre archivo de texto de salida), entero que indica si se escriben visibildades por hijo o no
Salida: 
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
	archivo=fopen("entrada.csv","r");
	/*
	archivo=fopen(name,"r");
	if(archivo == NULL){
		printf("Error abriendo archivo\n");
		exit(1);
	}
	*/
	int j = 0;
    void * a = 0;
    printf("DISCOS:%d\n",discos );
    pthread_t hebras[discos];
    Monitor * monitores = (Monitor *)malloc(discos* sizeof(Monitor));
    //Se inicializan los monitores
    //Se crean las hebras en base al número de discos
    for (int i = 0; i < discos; ++i)
    {
    	pthread_cond_init(&monitores[i].noLleno,NULL);
		pthread_cond_init(&monitores[i].lleno,NULL);
		pthread_mutex_init(&monitores[i].mutex,NULL);
    	monitores[i].tamanoBuffer = tamBuffer;
		monitores[i].agregados = 0;
		monitores[i].buffer = (Datos*)malloc(sizeof(Datos)*tamBuffer);
		monitores[i].parciales = (double*)malloc(sizeof(double)*5);
		pthread_create(&hebras[i],NULL,prueba, &monitores[i]);
    }

    while (fgets(cadena,500,archivo)!=NULL){ 
			sscanf(cadena,"%lf,%lf,%lf,%lf,%lf",&u,&v,&r,&i,&ruido);
            data[0] = u;
            data[1] = v;
            data[2] = r;
            data[3] = i;
            data[4] = ruido;
            int disc = disco(v,u);
			agregar(&monitores[disc],data);
			printf("disco: %d\n", disc);    
		}
		helper = 0;
		for(j = 0; j<discos;j++){

			pthread_mutex_unlock(&monitores[j].mutex);
			pthread_cond_signal(&monitores[j].lleno);
			pthread_cond_signal(&monitores[j].noLleno);
		}


		

    j = 0;
    //Para que las hebras no mueran antes de terminar
    while(j < discos){
    	pthread_join(hebras[j],NULL);
    	if((monitores[j].parciales[4]) > 0){
    	prop[j][0] = (monitores[j].parciales[0])/(monitores[j].parciales[4]);
		prop[j][1] = (monitores[j].parciales[1])/monitores[j].parciales[4];
		prop[j][2] = (monitores[j].parciales[2]);
		prop[j][3] = (monitores[j].parciales[3]);
		prop[j][4] = monitores[j].parciales[4];
	}
		else{
			prop[j][0] = 0.0;
			prop[j][1] = 0.0;
			prop[j][2] = 0.0;
			prop[j][3] = 0.0;
		}



    	j++;
    }

    writeFile(nombreSalida,b);
}




void vaciar(Monitor *m){
	printf("M agregados: %d\n",m->agregados);
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
	if(helper == 0){
		//Agregar Mutex
	//	printf("i\n");
/*		prop[disco][0] = m->parciales[0];
		prop[disco][1] = m->parciales[1];
		prop[disco][2] = m->parciales[2];
		prop[disco][3] = m->parciales[3];
		prop[disco][4] = m->parciales[4];
		lista = agregarNodo(lista, m->parciales[0],m->parciales[1],m->parciales[2],m->parciales[4],m->parciales[3]);         
		//a = propiedades()
		*/
	}


}

//Consumidor con monitor tipo C
void * prueba(void * monitor){

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




int main(int argc, char *argv[]) {
  int iflag = 0;
  int oflag = 0;
  int nflag = 0;
  int dflag = 0;
  int sflag = 0;
  int bvalue = 0;
  int index;
  int c;
  nombreSalida = malloc(sizeof(char)*30);
  char * nombreEntrada = malloc(sizeof(char)*30);

  opterr = 0;

  while ((c = getopt (argc, argv, "i:o:d:n:s:b")) != -1)
    switch (c)
      {
      case 'i':
        iflag = 1;
        break;
      case 'o':
        oflag = 1;
        break;
      case 'd':
        dflag = 1;
        break;
      case 'n':
        nflag = 1;
        break;
      case 's':
 	    sflag = 1;
 	    break;
      case 'b':
        bvalue = 1;
        break;
      case '?':
        if (optopt == 'c')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr,
                   "Unknown option character `\\x%x'.\n",
                   optopt);
        return 1;
      default:
        abort ();
      }
      b = bvalue;
   
    //Se asignan los valores de entrada a las variables
    strcpy(nombreEntrada,argv[2]);
    strcpy(nombreSalida,argv[4]);
    discos = atoi(argv[8]);
    if(discos == 0){
      printf("La cantidad de discos debe ser >= 1\n");
      exit(1);
    }
    
    ancho = atof(argv[6]);
    if(ancho <= 0.0){
      printf("El ancho debe ser mayor a 0");
      exit(1);
    }
    tamBuffer = atoi(argv[10]);
    
    //Se inicializa la lista para almacenar las visibilidades
		helper=1;
	
   
	//Se asigna memoria para el arreglo en el cual se 
	//almacenarán los datos de los discos
	prop = (double**)malloc(sizeof(double*)*(discos));
	for(int j=0; j<discos;j++){
		prop[j] = (double*)malloc(sizeof(double)*5);
	}

    readFile(nombreEntrada,nombreSalida,bvalue,tamBuffer);

    //

   

    /*


    
    readFile(nombreEntrada,nombreSalida,bvalue);
    
    limpiarPipes();
*/
    return 0;
    
}