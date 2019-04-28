#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Nodo{
	float v;
	float u;
	float w;
	float i;
	float r;
	struct Nodo*sig;

}Nodo;

typedef struct Lista{
	Nodo * inicio;
	Nodo * fin;

	
}Lista;

void inicializar(Lista * lista){
	lista->inicio = NULL;
	lista->fin = NULL;
}


Lista*agregarNodo(Lista*lista, float v, float u, float w, float i, float r){
	Nodo*nodo;
	nodo=(Nodo*)malloc(sizeof(Nodo));
	nodo->v=v;
	nodo->u=u;
	nodo->w=w;
	nodo->i=i;
	nodo->r=r;

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


//N corresponde a las visibilidades del disco
float propiedades(Lista* lista, int tipo, int N){
	float pot = 0;
	float sumW = 0;
	float sumI = 0;
	float sumR = 0;

	Nodo * actual = lista->inicio;

	while(actual!=NULL){
		sumW = sumW + actual->w;
		sumI = sumI + actual->i;		
		sumR = sumR + actual->r;
		pot = pot + sqrt(pow(actual->r,2) + pow(actual->u,2));
		actual = actual->sig;


	}
	

	//Media Real
	if(tipo == 0)
		return sumR/N;
	//Media imaginaria
	else if(tipo == 1)
		return sumI/N;
	//Potencia
	else if(tipo == 2)
		return pot;
	else
		return sumW;

}



int main(int argc, char const *argv[])
{

	Lista * lista=(Lista*)malloc(sizeof(Lista));
	inicializar(lista);
	lista = agregarNodo(lista,46.75563,-160.447845,-0.014992,0.005196,0.005011);
	lista = agregarNodo(lista,119.08387,-30.927526,0.016286,-0.001052,0.005888);
	lista = agregarNodo(lista,-132.906616,58.374054,-0.009442,-0.001208,0.003696);
	lista = agregarNodo(lista,-180.271179,-43.749226,-0.011654,0.001075,0.003039);
	lista = agregarNodo(lista,-30.299767,-126.668739,0.015222,-0.004145,0.007097);
	lista = agregarNodo(lista,-18.289482,28.76403,0.025931,0.001565,0.004362);



	return 0;
}