# makefile para Linux,  


Lab3 : lab3.o funciones.o 
	gcc -o lab3 lab3.o funciones.o -lm

lab2.o: lab3.c lab3.h 
	gcc -c lab3.c -lm

funciones.o: funciones.c lab3.h 
	gcc -c funciones.c -lm