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


pid_t parent;
char buffer[100];
int discos;
float ancho;
int**pipesIn;
int**pipesOut;
pid_t *childPids;
double **prop;


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
Descripción: Función que se encarga de inicializar los pipes de entrada y salida dependiendo la cantidad de discos
Entrada:
Salida:
*/
void inicializadPipes(){
	//se inicializan los arreglos de pipe
    //Pipes para que el hijo escriba
    pipesIn = (int**)malloc(sizeof(int*)*(discos));
    //Pipes para que el hijo lea
    pipesOut = (int**)malloc(sizeof(int*)*(discos));
    //Se inicializan los pipes
    for(int i = 0; i<discos; i++){
        pipesIn[i] = (int*)malloc(sizeof(int)*2);
        pipesOut[i] = (int*)malloc(sizeof(int)*2);
        pipe(pipesIn[i]); 
        pipe(pipesOut[i]);
    }
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

//Función que se utiliza para asignar todos los valores
//de un arreglo como -1
/*
Entrada: Arreglo de datos double de tamaño 5
Salida: 
*/
void end(double* a){
    for(int i = 0; i<5; i++){
        a[i] = -1.0;
    }
}

//función que lee el archivo de texto y se encarga de 
//mandar los datos a los hijos y recibir las respuestas
//de estos.
/*
Entrada: Puntero a char (nombre archivo de texto), puntero a char (nombre archivo de texto de salida), entero que indica si se escriben visibildades por hijo o no
Salida: 
*/
void readFile(char* name, char * nombreSalida, int b){

  //Se abre archivo de texto
	FILE * archivo;
	archivo=fopen(name,"r");
	if(archivo == NULL){
		printf("Error abriendo archivo\n");
		exit(1);
	}

    //Se crean tantos hijos como discos
    for(int i = 0; i<discos; i++){
        childPids[i] = fork();
       
        if(childPids[i] == 0){
            dup2(pipesOut[i][1],STDOUT_FILENO);
            dup2(pipesIn[i][0],STDIN_FILENO);
            close(pipesOut[i][0]);
            close(pipesIn[i][1]);    
            execl("./Vis","./Vis",(char *)NULL);
            
        }
		//Se cierran los canales correspondientes
		//a los pipes respectivos para respetar el esquema
		//de comunicación bidireccional.
		close(pipesOut[i][ESCRITURA]);
      	close(pipesIn[i][LECTURA]);
    }
    
	char cadena[500];
	int opcion;
	double v;
	double u;
	double r;
	double i;
	double ruido;
  double data[5];

	//Se lee del archivo de texto y se envía datos al hijo
	//correspondiente
	while (fgets(cadena,500,archivo)!=NULL){ 
		sscanf(cadena,"%lf,%lf,%lf,%lf,%lf",&u,&v,&r,&i,&ruido);
        data[0] = u;
        data[1] = v;
        data[2] = r;
        data[3] = i;
        data[4] = ruido;
        int disc = disco(v,u);
        write(pipesIn[disc][ESCRITURA],&data,sizeof(data));
	}
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
}


int main(int argc, char *argv[]) {

  int iflag = 0;
  int oflag = 0;
  int nflag = 0;
  int dflag = 0;
  int bvalue = 0;
  int index;
  int c;
  char * nombreSalida = malloc(sizeof(char)*30);
  char * nombreEntrada = malloc(sizeof(char)*30);

  opterr = 0;

  while ((c = getopt (argc, argv, "i:o:n:d:b")) != -1)
    switch (c)
      {
      case 'i':
        iflag = 1;
        break;
      case 'o':
        oflag = 1;
        break;
      case 'n':
        nflag = 1;
        break;
      case 'd':
        dflag = 1;
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
    discos = atoi(argv[6]);
    if(discos == 0){
      printf("La cantidad de discos debe ser >= 1\n");
      exit(1);
    }
    
    ancho = atof(argv[8]);
    if(ancho <= 0.0){
      printf("El ancho debe ser mayor a 0");
      exit(1);
    }
    childPids = malloc(sizeof(pid_t)*discos);

    parent = getpid();

    inicializadPipes();
    
    readFile(nombreEntrada,nombreSalida,bvalue);
    
    
    return 0;
    
}