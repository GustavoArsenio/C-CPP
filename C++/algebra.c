#include<stdio.h>
#include <stdlib.h>
#include "algebra.h"

struct MatrixM buildV(struct MatrixM Matrix){
	int showPosX=0,showPosY=0;
	setbuf(stdin,NULL);
	printf("\n\n Digite o nome: ");
	gets(Matrix.nome);
	printf("\t >>> %s",Matrix.nome);
	printf("\n\n Digite a quantidade de linhas: ");
	scanf("%d",&Matrix.x);
	printf("\n Digite a quantidade de colunas: ");
	scanf("%d",&Matrix.y);
	printf("\n\n\n \t x: %d \n \t y: %d",Matrix.x,Matrix.y);
	Matrix.Matriz = instMatrix(Matrix.Matriz,Matrix.x,Matrix.y);
	printf("\n \n \t Bem vindo ao MatrixBuilder! \t \n\n\n");
	for(int i = 0; i < Matrix.x; ++i)
	{
		for(int j=0; j < Matrix.y; j++){
			if(showPosX>=Matrix.x){
				showPosX = 0;
				showPosY++;
			}
			printf("\n\nDigite o valor M[%d][%d]: ",showPosX,showPosY);
			scanf("%d",&Matrix.Matriz[i][j]);
			showPosX++;
		}
	}	
	return Matrix;
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
struct MatrixM sumMatrix(struct MatrixM pMatrizResult,struct MatrixM pMatriz, struct MatrixM pMatriz2){
	pMatrizResult.Matriz = instMatrix(pMatrizResult.Matriz,pMatriz.x,pMatriz.y);
	setbuf(stdin,NULL);
	printf("\n\n Digite o nome para a matriz de soma: ");
	gets(pMatrizResult.nome);
	for (int i = 0; i <pMatriz.x; ++i)
	{
		for(int j =0; j<pMatriz.y; j++){
			pMatrizResult.Matriz[i][j] = (int)pMatriz2.Matriz[i][j] + (int)pMatriz.Matriz[i][j];
		}
	}
	pMatrizResult.x = pMatriz.x;
	pMatrizResult.y = pMatriz.y;
	printf("\n\n\t O resultado da matriz \"%s\" e: ",pMatrizResult->nome );
	return pMatrizResult;
}
int **transpose(int **pMatriz,int x,int y){
	int **MTransp;
	MTransp	= instMatrix(MTransp,y,x);
	for(int i =0;i<x;i++){
		for(int j =0;j<y;j++){
			MTransp[j][i]=pMatriz[i][j];
		}
	}
	return MTransp;
}
int **instMatrix(int **pMatriz, int x, int y){
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

int **multMatrix(struct MatrixM pMatriz, struct MatrixM pMatriz2){
	//printf("\n\n\n \t\t\t  >>>>>>>>>>>>>  Entrou <<<<<<<<<<<<<<<<");
	int **matrixResult = instMatrix(matrixResult,pMatriz.x,pMatriz2.y),auxX=0,auxXf=0, auxY=0,resultM=0;
	//printf("\n\n\n \t\t\t  >>>>>>>>>>>>>  Primeira instancia <<<<<<<<<<<<<<<<");
	//showV(pMatriz.Matriz,pMatriz.x,pMatriz.y);
	int **pMatrizTransp = instMatrix(pMatrizTransp,pMatriz.y,pMatriz.x);
	//pMatrizTransp = transpose(pMatriz.Matriz,pMatriz.x,pMatriz.y);
	//printf("\n\n\n \t\t\t  >>>>>>>>>>>>>  Segunda instancia <<<<<<<<<<<<<<<<");
	// pMatrizTransp = transpose(pMatriz,x,y);
	//printf("\n\n\n \t\t\t  >>>>>>>>>>>>>  transposta feita <<<<<<<<<<<<<<<<");
	showV(pMatrizTransp,pMatriz.y,pMatriz.x);
	showV(pMatriz2.Matriz,pMatriz2.x,pMatriz2.y);
	if(pMatriz.y==pMatriz2.x)
	{
		//printf("\n\n\n \t\t\t  >>>>>>>>>>>>>  Entrou no IF <<<<<<<<<<<<<<<<");
		for(int k =0;k<pMatriz.x;k++)
		{
			for(int i =0;i<pMatriz2.x;i++)
			{
				for(int j =0;j<pMatriz2.y;j++)
				{
					if( !(auxX%pMatriz2.y) ){
						auxX=0;
					}
					//printf(" \n\n >> %d * %d ",pMatriz2.Matriz[j][i],pMatrizTransp[auxX][k]);
					resultM+=pMatriz2.Matriz[j][i] * pMatrizTransp[auxX][k];
					auxX++;
				}
				if( !(auxX%pMatriz.y) ){
						auxX=0;
				}
				if( !(auxXf%pMatriz.y) ){
						auxXf=0;
				}
				//printf(" %d \n \t ---",resultM);
				matrixResult[auxY][auxXf] = resultM;
				//printf("\n \t  matrixResult: auxY [%d] auxXf [%d]  = value [%d]",auxY,auxXf,matrixResult[auxY][auxXf]);
				auxXf++;
				resultM=0;
			}
			auxY++;
			//printf("\n\n");
		}
	}else{
		printf("\n\n \t **** >>>> Error: Invalid Matrix Dimensions <<<< **** ");		
	}
	showV(matrixResult,pMatriz.x,pMatriz2.y);
	return 0;
}