#include "lab2.h"

pid_t parent;
char buffer[100];
int discos;
float ancho;
int tamBuffer;
int**pipesIn;
int**pipesOut;
pid_t *childPids;
double **prop;

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
			printf("Soy el hijo de Pid %i y procesé %d visibilidades\n",childPids[i],(int)prop[i][4] );
		}
    fprintf(archivoSalida,"\n");
		i++;
	}	
	fclose(archivoSalida);
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
    	monitores[i].tamanoBuffer = tamBuffer;
		monitores[i].agregados = 0;
		monitores[i].buffer = (Lista*)malloc(sizeof(Datos)*tamBuffer);
		inicializar(monitores[i].buffer);
		
    }
	struct Parametros * p = malloc (sizeof (struct Parametros)*discos);

     while (fgets(cadena,500,archivo)!=NULL){ 
		sscanf(cadena,"%lf,%lf,%lf,%lf,%lf",&u,&v,&r,&i,&ruido);
        data[0] = u;
        data[1] = v;
        data[2] = r;
        data[3] = i;
        data[4] = ruido;
        printf("ddd%lf\n",data[0] );
        int disc = disco(v,u);
		p[disc].monitores = &monitores[disc];
		p[disc].data = malloc(sizeof(double)*5);
		memcpy(p[disc].data,data,sizeof(data));
		printf("eee%lf\n",data[0] );
        pthread_create(&hebras[disc],NULL,prueba, (void *) &p[disc]);
        
	}
    	

    j = 0;
    //Para que las hebras no mueran antes de terminar
    while(j < discos){
    	pthread_join(hebras[j],NULL);
    	j++;
    }
    
	



  /*

  //Se inicializa mensaje de fin
	end(data);
	//Se envía mensaje de fin
	for(int j=0;j<discos;j++){
		write(pipesIn[j][ESCRITURA],&data,sizeof(data));
	}

  //Se cierran los pipes en que el padre escribe
	for(int j=0;j<discos;j++){
		close(pipesIn[j][ESCRITURA]);
	}
		
	//Se asigna memoria para el arreglo en el cual se 
	//almacenarán los datos de los discos
	prop = (double**)malloc(sizeof(double*)*(discos));
	for(int j=0; j<discos;j++){
		prop[j] = (double*)malloc(sizeof(double)*5);
	}

	//Acá se obtienen los datos de los discos
	for(int j = 0; j<discos; j++){
		read(pipesOut[j][LECTURA],prop[j], sizeof(double)*5);
	}
	
	fclose(archivo);
	writeFile(nombreSalida,b);
	*/
}

void * prueba(void * parametros){
	struct Parametros * p;
	p = (struct Parametros *) parametros;
	if(p->monitores->tamanoBuffer == p->monitores->agregados){
		pthread_cond_signal(&p->monitores->noLleno);
		pthread_cond_wait(&p->monitores->lleno,NULL);

	}
	else{
//agregar
		p->monitores->agregados++;
		if(p->monitores->agregados == p->monitores->tamanoBuffer){
			pthread_cond_signal(&p->monitores->lleno);
			pthread_cond_wait(&p->monitores->noLleno,NULL);
		}

	}
	

	printf("%d\n",1 );
	void * a;
	return a;
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
  char * nombreSalida = malloc(sizeof(char)*30);
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
    Lista * lista = malloc(sizeof(Lista));
    inicializar(lista);

    readFile(nombreEntrada,nombreSalida,bvalue,tamBuffer);

    //

   

    /*


    
    readFile(nombreEntrada,nombreSalida,bvalue);
    
    limpiarPipes();
*/
    return 0;
    
}