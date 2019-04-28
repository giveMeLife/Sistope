#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


float raiz(float a, float b){
	return sqrt(a*a + b*b);
}

int main(int argc, char const *argv[])
{
	FILE * archivo;
	char cadena[500];
	char archivo1[32];
	int opcion;
	float v;
	float u;
	float w;
	float i;
	float r;



	archivo=fopen("entrada.csv","r");
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
	


	while (fgets(cadena,500,archivo)!=NULL){ 
		sscanf(cadena,"%f,%f,%f,%f,%f",&v,&u,&w,&i,&r);
		printf("%f-%f-%f-%f-%f\n",v,u,w,i,r);


	}
			fclose(archivo);
		



	return 0;
}