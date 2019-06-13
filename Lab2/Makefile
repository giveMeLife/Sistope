# makefile para Linux,  


Lab2 : lab2.o funciones.o 
	gcc -o lab2 lab2.o funciones.o -lm -lpthread

lab2.o: lab2.c lab2.h 
	gcc -c lab2.c -lm -lpthread

funciones.o: funciones.c lab2.h 
	gcc -c funciones.c -lm -lpthread