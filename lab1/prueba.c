#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>


#define LECTURA 0
#define ESCRITURA 1


int main(int argc, char *argv[]) {
	//Arreglo de 2 enteros, la función pipe necesita de un array de 2 int
	/*int *pipes = (int *)malloc(2*sizeof(int));
	
	//Se llama a la función pipe
	pipe(pipes);
	//LECTURA = 0 y ESCRITURA = 1. Están definidos en constantes más arriba
	printf("Pipes tiene los siguientes valores: %d, %d\n", 
		   pipes[LECTURA], pipes[ESCRITURA]);
	
	//Declaro un string de ejemplo
	char stringEjemplo[10] = "hola";
	
	//Escribo en el pipe creado lo que hay en ese string. Envío 10 bytes en ese ejemplo
	write(pipes[ESCRITURA], stringEjemplo, 10*sizeof(char));
	printf("Se escribió\n");
	
	//Reservo memoria para un segundo string, en este se va a guardar lo que sea leído
	char *string2 = (char *)malloc(10*sizeof(char));
	
	//Hago que sea un string vacio escribiendo un 0 en el primer elemento.
	string2[0] = '\0';
	printf("String2 tiene lo siguiente: %s\n", string2); //Va a mostrar que está vacío
	printf("Se va a leer\n");
	
	//La función read necesita el descriptor de lectura, almacenado en el primer elemento del arreglo que se le pasó a la función pipe
	//Read devuelve la cantidad de bytes correctamente leídos y deposita lo leído en el array del 2° argumento, string2 en este caso
	int cuantoLeyo = read(pipes[LECTURA], string2, 10*sizeof(char));
	printf("Se leyeron %d bytes\n", cuantoLeyo);
	printf("Se leyó: %s\n", string2);
	//FIN PARTE DE CREACIÓN DE PIPES
	*/
	//En lo anterior se mostró como escribir y leer cosas desde un pipe, pero todo esto se hizo en un mismo proceso, lo cual no es de mucha utilidad, la idea es comunicar procesos diferentes.
	
	//Ahora se va a mostrar lo mismo, pero para comunicar 2 procesos (o más) diferentes
	
	//int valorFork = fork(); //Se crea un nuevo proceso, lo único que diferencia al padre con el hijo es el valor de retorno de fork
	/*if (valorFork == 0) { //Hijo
		close(pipes[LECTURA]); //El hijo no va a leer, por lo tanto se cierra su descriptor
		char *string3 = (char *)malloc(10*sizeof(char)); //Declaro memoria para un string
		string3[0] = '\0'; //Hago que sea un string vacío
		strcpy(string3, "sustope!!"); //copio algo en ese string
		write(pipes[ESCRITURA], string3, 10*sizeof(char)); //Escribo en el pipe (esto lo ejecuta el proceso hijo)
		printf("Hijo: Se escribió\n");
	}
	else { //Padre
		close(pipes[ESCRITURA]); //Como el padre no va a escribir, cierra el descriptor de escritura
		int cuantoLeyo2 = read(pipes[LECTURA], string2, 10*sizeof(char));
		
		//Acá se mostrará que lo que envió el proceso hijo, es decir el string "sustope!!" es leído por el proceso padre
		printf("Padre: Se leyeron %d bytes\n", cuantoLeyo);
		printf("Padre: Se leyó: %s\n", string2);
		
	}*/
    /*pid_t pid = getpid();
    int cantidad = 5;
    for(int i = 0; i<cantidad; i++){
        if(getpid() == pid){
            int hijo = fork();
            if(hijo == 0){
                execvp("./e",NULL);
            }
            
        }
    }*/
    //wait(NULL);
    //lectura->0
    //escritura->1
    int *pipefd = (int*)malloc(sizeof(int)*2);
    int *pipefd2 = (int*)malloc(sizeof(int)*2);
    pipe(pipefd);
    pipe(pipefd2);
    int pid = fork();
    if(pid == 0){
        printf("hola\n");
        dup2(pipefd[1],STDOUT_FILENO);
        /*printf("nose\n");
        dup2(pipefd2[0],STDIN_FILENO);
        printf("chao\n");
        close(pipefd[0]);
        close(pipefd2[1]);*/
        dup2(pipefd2[0],STDIN_FILENO);

        //printf("hola\n");STDOUT_FILENO
        //printf("hola\n");
        execv("./a",NULL);        
    }
    char buffer[100];
    write(pipefd2[0], "Hola hijo\n", 11);
    wait(NULL);
    read(pipefd[1], buffer, 100);
    printf("Padre: %s\n", buffer);
    

	
	return 0;
}