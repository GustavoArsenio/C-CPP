#include <stdio.h>
#include <stdlib.h>
#include "algebra.h"

int main(int argc, char const *argv[])
{
	struct MatrixM Matriz,Matriz2,pMatrizResult,matrizTransp;
	int x,y, **Matriz,**Matriz2,**pMatrizResult, **matrizTransp;
	printf("\n\n Digite a quantidade de linhas: ");
	scanf("%d",&x);
	printf("\n Digite a quantidade de colunas ");
	scanf("%d",&y);
	printf("\n\n\n \t x: %d \n \t y: %d",x,y);
	Matriz = buildV(Matriz,x,y);
	showV(Matriz,x,y);
	Matriz2 = buildV(Matriz2,x,y);
	showV(Matriz2,x,y);
	pMatrizResult=sumMatrix(pMatrizResult,Matriz,Matriz2,x,y);
	showV(pMatrizResult,x,y);
	matrizTransp = transpose(Matriz,x,y);	
	showV(matrizTransp,y,x);
	printf("\n\n >>> Terminando Matriz: ");
	liberaM(Matriz,x);
	printf("\n | 1- OK ");
	liberaM(Matriz2,x);
	printf("\n | 2- OK ");
	liberaM(pMatrizResult,x);
	printf("\n | 3- OK ");
	liberaM(matrizTransp,y);
	printf("\n | 4- OK ");
	return 0;
}