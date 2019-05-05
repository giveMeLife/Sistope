#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>
#include <ctype.h>

/*
Ahora solo falta hacer que los hijos devuelvan los datos específicos
Lo que se puede hacer, es que se devuelva un array con los datos de la visibilidad
y con el pid del hijo que lo manda.
Así que solo hay que escribir desde los hijos al padre y era
TE QUIERO MUCHO!! <3 <3 3<


*/

#define LECTURA 0
#define ESCRITURA 1
float global = 0.0;
pid_t parent;
char buffer[100];
int discos = 5;
int ancho = 2;
int**pipesIn;
int**pipesOut;
pid_t *childPids;
float raiz(float a, float b){
	return sqrt(a*a + b*b);
}

//Función que indica en qué disco se encuentra la visibilidad
int disco(float a, float b){
    float punto = raiz(a,b);
    float disc = punto/(float)ancho;
    if(disc>=(float)(discos*ancho)){
        return discos;
    }
    return (int)disc;

}

//Función que se utiliza para asignar todos los valores
//de un arreglo como -1
void end(float* a){
    for(int i = 0; i<6; i++){
        a[i] = -1.0;
    }
}

//función que lee el archivo
void readFile(char* name){
    //Se crean tantos hijos como discos
    for(int i = 0; i<discos+1; i++){
        childPids[i] = fork();
       
        if(childPids[i] == 0){
            dup2(pipesOut[i][1],STDOUT_FILENO);
            dup2(pipesIn[i][0],STDIN_FILENO);
            close(pipesOut[i][0]);
            close(pipesIn[i][1]);    
            execl("./son","./son",(char *)NULL);
            
        }
        printf("%d\n",childPids[i]);
        
        // close(pipesOut[i][ESCRITURA]);
        // close(pipesIn[i][LECTURA]); 
    }
     for(int j = 0; j<discos+1; j++){
         close(pipesOut[j][ESCRITURA]);
         close(pipesIn[j][LECTURA]);
        

     }
    
    FILE * archivo;
	char cadena[500];
	char archivo1[32];
	int opcion;
	float v;
	float u;
	float w;
	float i;
	float r;

	archivo=fopen(name,"r");
	while(archivo==NULL){ // Si el archivo no abre, el programa termina
		printf("Fallo la apertura del archivo ¿Qué desea hacer a continuación?\n1)Volver a ingresar nombre\n2)Terminar el programa\n");
		if(opcion==1){
			printf("Ingrese el nombre del archivo con las instrucciones: \n");
			scanf("%s",archivo1);
			archivo=fopen(archivo1,"r");
		}
		else if(opcion==2) {
			exit(1);
			
		}
		else{
			printf("Ingrese una opcion valida\n");
			
		}
	}
    float data[6];
    //GLOBAL es utilizado para que el hijo sepa si sigue
    //leyendo el mismo dato o no.
	while (fgets(cadena,500,archivo)!=NULL){ 
		sscanf(cadena,"%f,%f,%f,%f,%f",&v,&u,&w,&i,&r);
		//printf("%f-%f-%f-%f-%f\n",v,u,w,i,r);
        data[0] = v;
        data[1] = u;
        data[2] = w;
        data[3] = i;
        data[4] = r;
        data[5] = global + 0.0;
        global = global + 1.0;
        int disc = disco(v,u);
        //printf("Soy el disco: %d\n", disc);
         write(pipesIn[disc][ESCRITURA],&data,sizeof(data));
	}

    
    end(data);
    for(int j=0;j<discos+1;j++){
       write(pipesIn[j][1],&data,sizeof(data));
    }
    for(int j=0;j<discos+1;j++){
        close(pipesIn[j][1]);
    }
    int status;
    
    // for(int j = 0; j<discos+1; j++){
    //     //printf("%d\n",j);
    //     close(pipesIn[j][ESCRITURA]);
    // }
    //se espera el término de los hijos
    
    int o;

    
    for(int j = 0; j<discos+1;j++){
        read(pipesOut[j][0],&o,sizeof(o));
        printf("Mi hijo me mandó: %d\n",o);
        close(pipesOut[j][LECTURA]);
    }
	fclose(archivo);
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
    printf("Discos: %d\n", discos);
    parent = getpid();
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
    
    
    readFile(nombreEntrada);
    
    
    return 0;
    
}