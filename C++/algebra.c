#include<stdio.h>
#include <stdlib.h>
#include "algebra.h"

int **buildV(int **pMatriz, int x, int y){
	int showPosX=0,showPosY=0;
	pMatriz = popMatrix(pMatriz,x,y);
	printf("\n \n \t Bem vindo ao MatrixBuilder! \t \n\n\n");
	for(int i = 0; i < x*y; ++i)
	{
		if(showPosX>=x){
			showPosX = 0;
			showPosY++;
		}
		printf("\n\nDigite o valor M[%i][%i]: ",showPosX,showPosY);
		scanf("%i",&pMatriz[i]);
		showPosX++;
	}	
	return pMatriz;
}
void showV(int **pMatrizS, int x, int y){
	printf("\n\n\n\n \t\t\t ***  Mostrando Matriz *** ");
	for(int i = 0; i < x*y; i++)
	{	
		if(!(i%x))
		{
			printf("\n");
		}
		printf(" | %i | ",pMatrizS[i]);
	}
	printf("\n\n\n\n \t\t *** Fim mostrar Matriz*** ");
}
void soma(int **pMatriz,int x,int y){
	showV(pMatriz,x,y);
	int aux=0;
	printf("\n\n Resultado: %i",aux);
	printf("\n\n x: %i \t y: %i",x,y);
	for (int i = 0; i < x*y; ++i)
	{
	printf("\n\n Posicao: %i, valor %d",i,pMatriz[i]);
		aux = aux + (int) pMatriz[i];
	}
}
int **sumMatrix(int **pMatrizResult,int **pMatriz, int **pMatriz2, int x,int y){
	pMatrizResult = popMatrix(pMatrizResult,x,y);
	for (int i = 0; i < x*y; ++i)
	{
		pMatrizResult[i] = (int)pMatriz2[i] + (int)pMatriz[i];
	}
	return pMatrizResult;
}
int **transpose(int **pMatriz,int x,int y){
	int **MTransp;
	MTransp	= popMatrix(MTransp,y,x);
	for(int i =0;i<y;i++){
		for(int j =0;j<x;j++){
			MTransp[y*j+i]=pMatriz[i*x+j];
		}
	}
	return MTransp;
}
int **popMatrix(int **pMatriz, int x, int y){
	pMatriz = (int **) malloc (x *sizeof(int *));
	for(int i = 0; i < x; ++i)
	{
		pMatriz[i] = (int *) malloc(y * sizeof(int));
	} 
	return pMatriz;
}

int **multMatrix(int **pMatriz,int x,int y, int **pMatriz2, int x2, int y2){
	int **matrixResult = popMatrix(matrixResult,x,y2),auxX,auxY,auxX2,,auxY2;
	for(int i =0;i<(x*y2);i++){
		while(;){
			
		}
		matrixResult[i]=aux;
	}
}