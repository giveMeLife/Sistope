#include "lab3.h"



/*
Parámetros de entrada: 
 ./lab3 -u subida.csv -d bajada.csv -o ordinario.csv -n npisos -e nascensores -b
• -u: nombre de archivo de entrada con las llamadas del horario de la mañana (bajada)
• -d: nombre de archivo de entrada con las llamadas del horario de la tarde (subida)
• -o: nombre de archivo de entrada con las llamadas de cualquier horario (random)
• -n: cantidad de pisos del edificio
• -e: cantidad de ascensores
• -b: bandera o flag que permite indicar si se quiere ver por consola los resultados de salida.
*/

int main(int argc, char *argv[]) {
  int uflag = 0;
  int dflag = 0;
  int oflag = 0;
  int nflag = 0;
  int eflag = 0;
  int bvalue = 0;
  int c;
  int pisos = 0;
  int ascensores = 0;

  char * nombreSalida = malloc(sizeof(char)*30);
  char * nombreSubida = malloc(sizeof(char)*30);
  char * nombreBajada = malloc(sizeof(char)*30);
  char * nombreOrdinario = malloc(sizeof(char)*30);

  opterr = 0;
//Se solicitan los datos
  while ((c = getopt (argc, argv, "u:d:o:n:e:b")) != -1)
    switch (c)
      {
      case 'u':
        strcpy(nombreSubida, optarg);
        uflag = 1;
        break;
      case 'd':
        strcpy(nombreBajada, optarg);
        dflag = 1;
        break;
      case 'o':
        strcpy(nombreOrdinario, optarg);
        oflag = 1;
        break;
      case 'n':
        nflag = 1;
        pisos = atoi(optarg);
        break;
      case 'e':
        ascensores = atoi(optarg);
 	      eflag = 1;
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
    if(pisos < 1){
      printf("La cantidad de pisos debe ser >= 1\n");
      exit(1);
    }
    
    if(ascensores <= 0){
      printf("La cantidad de ascensores debe ser mayor a 0");
      exit(1);
    }
    

	//Se lee el archivo
    readFile(nombreSubida,nombreBajada,nombreOrdinario,pisos,ascensores,bvalue);
	  writeFile("salida.txt",bvalue);
    return 0;
    
}