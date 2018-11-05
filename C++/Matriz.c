#include <stdio.h>
#include <stdlib.h>
#include "algebra.h"

int main(int argc, char const *argv[])
{
	struct MatrixM Matriz,Matriz2,pMatrizResult,matrizTransp;
	//int x,y,x2,y2,**Matriz,**Matriz2,**pMatrizResult,**matrizTransp;
	setbuf(stdin,NULL);
	printf("\n\n Digite o nome: ");
	gets(Matriz.nome);
	printf("\t >>> %s",Matriz.nome);
	printf("\n\n Digite a quantidade de linhas: ");
	scanf("%d",&Matriz.x);
	printf("\n Digite a quantidade de colunas: ");
	scanf("%d",&Matriz.y);
	printf("\n\n\n \t x: %d \n \t y: %d",Matriz.x,Matriz.y);
	Matriz.Matriz = buildV(Matriz);
	Matriz.matrizTransp = transpose(Matriz.Matriz,Matriz.x,Matriz.y);
	showV(Matriz.Matriz,Matriz.x,Matriz.y);
	printf("\n\n **************************** ");
	showV(Matriz.matrizTransp,Matriz.y,Matriz.x);
	printf("\n\n **************************** ");
	setbuf(stdin,NULL);
	printf("\n\n Digite o nome pM2: ");
	gets(Matriz2.nome);
	printf("\t >>> %s",Matriz2.nome);
	printf("\n\n Digite a quantidade de linhas pM2: ");
	scanf("%d",&Matriz2.x);
	printf("\n Digite a quantidade de colunas pM2: ");
	scanf("%d",&Matriz2.y);
	Matriz2.Matriz = buildV(Matriz2);
	showV(Matriz2.Matriz,Matriz2.x,Matriz2.y);
	pMatrizResult=sumMatrix(pMatrizResult,Matriz,Matriz2);
	showV(pMatrizResult.Matriz,pMatrizResult.x,pMatrizResult.y);
	printf("\n\n\n \t\t\t  >>>>>>>>>>>>>  Comecando A multiplicacao de matrizes <<<<<<<<<<<<<<<<");
	multMatrix(Matriz,Matriz2);
	printf("\n\n >>> Terminando Matriz: ");
	liberaM(Matriz.Matriz,Matriz.x);
	printf("\n | 1- OK ");
	liberaM(Matriz2.Matriz,Matriz2.x);
	printf("\n | 2- OK ");
	liberaM(pMatrizResult.Matriz,pMatrizResult.x);
	printf("\n | 3- OK ");
	liberaM(Matriz.matrizTransp,Matriz.y);
	printf("\n | 4- OK ");
	return 0;
}