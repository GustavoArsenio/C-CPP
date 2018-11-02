#include<stdio.h>
#include <stdlib.h>
#include "algebra.h"

int **buildV(int **pMatriz, int x, int y){
	int showPosX=0,showPosY=0;
	pMatriz = popMatrix(pMatriz,x,y);
	printf("\n \n \t Bem vindo ao MatrixBuilder! \t \n\n\n");
	for(int i = 0; i < x; ++i)
	{
		for(int j=0; j < y; j++){
			if(showPosX>=x){
				showPosX = 0;
				showPosY++;
			}
			printf("\n\nDigite o valor M[%d][%d]: ",showPosX,showPosY);
			scanf("%d",&pMatriz[i][j]);
			showPosX++;
		}
	}	
	return pMatriz;
}
void showV(int **pMatrizS, int x, int y){
	printf("\n\n\n\n \t\t\t ***  Mostrando Matriz *** \n\n");
	for(int i = 0; i < x; i++)
	{	
	
		for(int j =0; j<y; j++){
			printf(" | %i | ",pMatrizS[i][j]);
		}
				printf("\n");
	}
	printf("\n\n\n\n \t\t *** Fim mostrar Matriz*** ");
}
void soma(int **pMatriz,int x,int y){
	printf(" \n>>>>>>>>> Entrou na soma");
	int aux=0;
	printf("\n\n x: %d \t y: %d",x,y);
	for (int i = 0; i < x; ++i)
	{
		for(int j =0; j<y; j++){
			printf("\n\n Posicao: %d %d, valor %d",i,j,pMatriz[i][j]);
			aux = aux + pMatriz[i][j];
		}
	}
	printf("\n\n Resultado: %d",aux);
}
int **sumMatrix(int **pMatrizResult,int **pMatriz, int **pMatriz2, int x,int y){
	pMatrizResult = popMatrix(pMatrizResult,x,y);
	for (int i = 0; i < x; ++i)
	{
		for(int j =0; j<y; j++){
			pMatrizResult[i][j] = (int)pMatriz2[i][j] + (int)pMatriz[i][j];
		}
	}
	return pMatrizResult;
}
int **transpose(int **pMatriz,int x,int y){
	int **MTransp;
	MTransp	= popMatrix(MTransp,y,x);
	for(int i =0;i<y;i++){
		for(int j =0;j<x;j++){
			MTransp[j][i]=pMatriz[i][j];
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
void liberaM(int **pMatriz,int x){
	for(int i = 0; i < x; i++)
	{
		free(pMatriz[i]);
	}
		free(pMatriz);
}
/*
int **multMatrix(int **pMatriz,int x,int y, int **pMatriz2, int x2, int y2){
	int **matrixResult = popMatrix(matrixResult,x,y2),auxX,auxY,auxX2,,auxY2;
	for(int i =0;i<(x*y2);i++){
		while(;){
			
		}
		matrixResult[i]=aux;
	}
}
*/