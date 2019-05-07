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
int ancho;
int**pipesIn;
int**pipesOut;
pid_t *childPids;
double **prop;


void inicializadPipes(){
	//se inicializan los pipes
    pipesIn = (int**)malloc(sizeof(int*)*(discos+1));
    pipesOut = (int**)malloc(sizeof(int*)*(discos+1));
    //pipes para que los hijos lean
    for(int i = 0; i<discos+1; i++){
        pipesIn[i] = (int*)malloc(sizeof(int)*2);
        pipesOut[i] = (int*)malloc(sizeof(int)*2);
        pipe(pipesIn[i]); 
        pipe(pipesOut[i]);
    }
}

double raiz(double a, double b){
	return sqrt(a*a + b*b);
}

//Función que indica en qué disco se encuentra la visibilidad
int disco(double a, double b){
    double punto = raiz(a,b);
    double disc = punto/(double)ancho;
    if(disc>=(double)(discos*ancho)){
        return discos;
    }
    return (int)disc;

}

//Función que se utiliza para asignar todos los valores
//de un arreglo como -1
void end(double* a){
    for(int i = 0; i<5; i++){
        a[i] = -1.0;
    }
}

//función que lee el archivo de texto y se encarga de 
//mandar los datos a los hijos y recibir las respuestas
//de estos.
void readFile(char* name){
	FILE * archivo;
	archivo=fopen(name,"r");
	if(archivo == NULL){
		printf("Error abriendo archivo\n");
		exit(1);
	}

    //Se crean tantos hijos como discos
    for(int i = 0; i<discos+1; i++){
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
	for(int j=0;j<discos+1;j++){
		write(pipesIn[j][ESCRITURA],&data,sizeof(data));
	}


	for(int j=0;j<discos+1;j++){
		close(pipesIn[j][ESCRITURA]);
	}
		
	//Se asigna memoria para el arreglo en el cual se 
	//almacenarán los datos de los discos
	prop = (double**)malloc(sizeof(double*)*(discos+1));
	for(int j=0; j<discos+1;j++){
		prop[j] = (double*)malloc(sizeof(double)*5);
	}

	//Acá se obtienen los datos de los discos
	for(int j = 0; j<discos+1; j++){
		read(pipesOut[j][LECTURA],prop[j], sizeof(double)*5);
	}
	

	//MUESTRA FUNCIONAMIENTO
	// printf("Se ha leído con éxito\n");
	// 	for(int j = 0; j<discos+1; j++){
	// 	printf("Mis datos son: MediaReal->%lf, MediaImaginaria->%lf, Potencia->%lf, RuidoTotal->%lf\n",prop[j][0],prop[j][1],prop[j][2],prop[j][3]);
	// }

	fclose(archivo);
}

void writeFile(){
	//Do something
}


int main(int argc, char *argv[]) {

  int iflag = 0;
  int oflag = 0;
  int nflag = 0;
  int dflag = 0;
  char *bvalue = NULL;
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
        bvalue = optarg;
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
    printf ("iflag = %d, oflag = %d, nflag = %d, dflag = %d,bvalue = %s\n",
              iflag, oflag, nflag, dflag,bvalue);
   
    //Se asignan los valores de entrada a las variables
    strcpy(nombreEntrada,argv[2]);
    strcpy(nombreSalida,argv[4]);
    discos = atoi(argv[6]);
    ancho = atoi(argv[8]);
    childPids = malloc(sizeof(pid_t)*discos);

    parent = getpid();

    inicializadPipes();
    
    readFile(nombreEntrada);
    
    
    return 0;
    
}