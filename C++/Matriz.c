
/*

###################################
# Construido por: Gustavo Arsenio #
# Curso: Engenharia da computação #
# RA: 0074830                     #
# UNASP Campus São Paulo          #
###################################

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "algebra.h"

int main(int argc, char const *argv[])
{
	struct conjuntoMatrizes matrizes;
	matrizes.tamanho =0;
	int selecionado, mSelected=0;
	while(1)
	{
		printf("\t\t\n________________      _____    ___________________ ");
		printf("\t\t\n|    |   \\      \\    /  _  \\  /   _____/\\______   \\");
		printf("\t\t\n|    |   /   |   \\  /  /_\\  \\ \\_____  \\  |     ___/");
		printf("\t\t\n|    |  /    |    \\/    |    \\/        \\ |    |    ");
		printf("\t\t\n|______/\\____|____/\\____|____/_________/ |____|    ");
		printf("\n\n\n\n\n");
		printf("\t\t\n_______________________________ ");
		printf("\t\t\n| 0) Sair                     |");
		printf("\t\t\n| 1) Add matriz               |");
		printf("\t\t\n| 2) Somar Matriz             |");
		printf("\t\t\n| 3) Multiplicar Matriz       |");
		printf("\t\t\n| 4) Fatorial                 |");
		printf("\t\t\n| 5) Determinante             |");
		printf("\t\t\n| 6) Transposta               |");
		printf("\t\t\n| 7) Exibir Matriz            |");
		printf("\t\t\n|_____________________________|\n\n");
		scanf("%d",&selecionado);
			printf(" \n\n \t >>>>>>>>>>> Opcao: %d",selecionado);
			if(selecionado==0){
				liberaAllM(matrizes);
				return 0;
			}else if(selecionado==1){
				matrizes = addMatriz(matrizes);
				printf(" \n\n \t >>>>>>>>>>>OK");
			}else if(matrizes.tamanho !=0 &&selecionado==7){
				printf("\n\n \t \t >>> Qual o número da sua matriz? <<<");
				scanf("%d",&mSelected);
				showMatriz(matrizes,mSelected);
				printf(" \n\n \t >>>>>>>>>>>OK");
			}else{
				printf(" \n\n \t >>>>>>>>>>> Error: Invalid Option ");
				return 0;
			}
		
	}
	return 0;
}
/*
struct MatrixM Matriz,Matriz2,pMatrizResult,pMultip;
	Matriz=buildV(Matriz);
	Matriz.matrizTransp = transpose(Matriz);
	showM(Matriz);
	printf("\n\n *** Entrando ** ");
	Matriz2 = buildV(Matriz2);
	pMatrizResult=sumMatrix(pMatrizResult,Matriz,Matriz2);
	showM(pMatrizResult);
	pMultip = multMatrix(Matriz,Matriz2);
	showM(pMultip);
	showM(Matriz);
	red(Matriz,1,0);
	printf("\n\n \t >>> A determinante da Matriz e: %d <<<<<<",determinante(Matriz));
	printf("\n\n >>> Terminando Program Matrix: ");
	liberaM(Matriz.Matriz,Matriz.x);
	printf("\n | 1- OK ");
	liberaM(Matriz2.Matriz,Matriz2.x);
	printf("\n | 2- OK ");
	liberaM(pMatrizResult.Matriz,pMatrizResult.x);
	printf("\n | 3- OK ");
	liberaM(Matriz.matrizTransp,Matriz.y);
	printf("\n | 4- OK ");
	liberaM(pMultip.Matriz,pMultip.x);
	printf("\n | 5- OK ");
*/