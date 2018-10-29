#include <stdio.h>
#include <stdlib.h>
#include "algebra.h"

int main(int argc, char const *argv[])
{
	int modoDeValid=0,x,y, **Matriz,**Matriz2,**pMatrizResult, **matrizTransp;
	printf("\n\n Digite a quantidade de linhas: ");
	scanf("%d",&y);
	printf("\n Digite a quantidade de colunas ");
	scanf("%d",&x);
	printf("\n\n\n \t x: %d \n \t y: %d",x,y);
	Matriz = buildV(Matriz,x,y);
	showV(Matriz,x,y);
	Matriz2 = buildV(Matriz2,x,y);
	showV(Matriz2,x,y);
	// printf("\n\n");
	// printf("\n\n\t *** Comeco populando Matriz de Resultado *** ");
	pMatrizResult=sumMatrix(pMatrizResult,Matriz,Matriz2,x,y);
	showV(pMatrizResult,x,y);
	// printf("\n\n\t *** Comeco transpondo Matriz 1*** ");
	matrizTransp = transpose(Matriz,x,y);
	// printf("\n\n\t *** Fim transpondo Matriz 1*** tamanho m1: %d tamanho M2: %d ",sizeof(Matriz),sizeof(matrizTransp));	
	showV(matrizTransp,y,x);
	determinante(Matriz,x,y);
	// printf("\n\n\t *** Fim programa *** ");
	free(Matriz);
	// printf("\n\n\t *** Fim liberacao de memoria M1 *** ");
	free(Matriz2);
	// printf("\n\n\t *** Fim liberacao de memoria M2 *** ");
	free(pMatrizResult);
	// printf("\n\n\t *** Fim liberacao de memoria pResult *** ");
	free(matrizTransp);
	// printf("\n\n\t *** Fim liberacao de memoria matrizTransp** ");
	return 0;
}