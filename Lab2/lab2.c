#include "lab2.h"



/*
Parámetros de entrada: 
 ./lab2 -i visibilidades.csv -o propiedades.txt -d ancho -n ndiscos -s tamañobuffer -b
• -i: nombre de archivo con visibilidades, el cual tiene como formato uv values i.csv con i = 0,1,2,....
• -o: nombre de archivo de salida
• -n: cantidad de discos
• -d: ancho de cada disco
• -s: tama˜no del buffer de cada monitor
• -b: bandera o flag que permite indicar si se quiere ver por consola la cantidad de visibilidades encontradas por cada proceso hijo. Es decir, si se indica el flag, entonces se muestra la salida por consola.
*/

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
//Se solicitan los datos
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
    
	helper=1;
	
    /*Se asigna memoria al arreglo en donde serán agregados luego las propiedades calculadas por
	  cada hebra
	 */
	prop = (double**)malloc(sizeof(double*)*(discos));
	for(int j=0; j<discos;j++){
		prop[j] = (double*)malloc(sizeof(double)*5);
	}

	//Se lee el archivo
    readFile(nombreEntrada,nombreSalida,bvalue,tamBuffer);
		writeFile(nombreSalida,b);
    return 0;
    
}