
/*

###################################
# Construido por: Gustavo Arsenio #
# Curso: Engenharia da computação #
# RA: 0074830                     #
# UNASP Campus São Paulo          #
###################################

*/

#include<stdio.h>
#include <stdlib.h>
#include <string.h>
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
void showV(struct MatrixM Matrix){
	printf("\n\n\n\n \t\t\t ***  Mostrando Matriz *** \n\n");
	for(int i = 0; i < Matrix.x; i++)
	{	
	
		for(int j =0; j<Matrix.y; j++){
			printf(" | %i | ",Matrix.Matriz[i][j]);
		}
				printf("\n");
	}
	printf("\n\n\n\n \t\t *** Fim mostrar Matriz*** ");
}
void showM(struct MatrixM Matrix){
	printf("\n\n **************************** ");
	printf("\n\n| >>> \t **** Matriz: %s *****",Matrix.nome);
	printf("\n| >>> \tde dimensao x:[%d] e y:[%d] ",Matrix.x,Matrix.y);
	printf("\n| >>>\t  Possui os valores de: ");
	showV(Matrix);
	printf("\n\n **************************** ");
	
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
	printf("\n\n\t O resultado da matriz %s e: ",pMatrizResult.nome );
	return pMatrizResult;
}
int **transpose(struct MatrixM pMatriz){
	int **MTransp;
	MTransp	= instMatrix(MTransp,pMatriz.y,pMatriz.x);
	for(int i =0;i<pMatriz.x;i++){
		for(int j =0;j<pMatriz.y;j++){
			MTransp[j][i]=pMatriz.Matriz[i][j];
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
void red(struct MatrixM Matrix, int x, int y){
	printf("\n\n\n\n \t\t\t ***  Mostrando Matriz *** \n\n");
	for(int i = 0; i < Matrix.x; i++)
	{	
		for(int j =0; j<Matrix.y; j++){
			if(i==x){
				i++;
				if(i==Matrix.x){
					break;
				}
			}			
			if(j==y){
				j++;
				if(j==Matrix.y){
					break;
				}
			}
			printf(" \n >>>>>>>>>>>>>>>>>> | %i | ",Matrix.Matriz[i][j]);
		}
	}
}

struct MatrixM multMatrix(struct MatrixM pMatriz, struct MatrixM pMatriz2){
	printf("\n\n\n \t\t\t  >>>>>>>>>>>>>  Entrou <<<<<<<<<<<<<<<<");
	struct MatrixM matrixResult;
	matrixResult.Matriz = instMatrix(matrixResult.Matriz,pMatriz.x,pMatriz2.y);
	int auxX=0,auxXf=0, auxY=0,resultM=0;
	int **pMatrizTransp = instMatrix(pMatrizTransp,pMatriz.y,pMatriz.x);
	pMatrizTransp = transpose(pMatriz);
	strcpy(matrixResult.nome,"Matriz_Multip");
	matrixResult.x = pMatriz.x;
	matrixResult.y = pMatriz2.y;
	if(pMatriz.y==pMatriz2.x)
	{
	
		for(int k =0;k<pMatriz.x;k++)
		{
			for(int i =0;i<pMatriz2.x;i++)
			{
				for(int j =0;j<pMatriz2.y;j++)
				{
					if( !(auxX%pMatriz2.y) ){
						auxX=0;
					}
					resultM+=pMatriz2.Matriz[j][i] * pMatrizTransp[auxX][k];
					auxX++;
				}
				if( !(auxX%pMatriz.y) ){
						auxX=0;
				}
				if( !(auxXf%pMatriz.y) ){
						auxXf=0;
				}
				matrixResult.Matriz[auxY][auxXf] = resultM;
				auxXf++;
				resultM=0;
			}
			auxY++;
		}
	}else{
		printf("\n\n \t **** >>>> Error: Invalid Matrix Dimensions <<<< **** ");		
		strcpy(matrixResult.nome,"**** >>>> Error: Invalid Matrix Dimensions <<<< ****");
	}
	return matrixResult;
}