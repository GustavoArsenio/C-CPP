
/*

	###################################
	# Construido por: Gustavo Arsenio #
	# Curso: Engenharia da computação #
	# RA: 0074830                     #
	# UNASP Campus São Paulo          #
	###################################

*/
	struct conjuntoMatrizes *filaMatrizes;
	int tamanho=0;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "algebra.h"

int main(int argc, char const *argv[])
{
	filaMatrizes=NULL;
	int selecionado, mSelected=0,mSelected2=0;
	while(1)
	{
		printf("\n\t\t________________      _____    ___________________ ");
		printf("\n\t\t|    |   \\      \\    /  _  \\  /   _____/\\______   \\");
		printf("\n\t\t|    |   /   |   \\  /  /_\\  \\ \\_____  \\  |     ___/");
		printf("\n\t\t|    |  /    |    \\/    |    \\/        \\ |    |    ");
		printf("\n\t\t|______/\\____|____/\\____|____/_________/ |____|    ");
		printf("\n\n\n\n\n");
		printf("\n\t\t\t_______________________________ ");
		printf("\n\t\t\t| 0) Sair                     |");
		printf("\n\t\t\t| 1) Add matriz               |");
		printf("\n\t\t\t| 2) Somar Matriz             |");
		printf("\n\t\t\t| 3) Multiplicar Matriz       |");
		printf("\n\t\t\t| 4) Fatorial                 |");
		printf("\n\t\t\t| 5) Determinante             |");
		printf("\n\t\t\t| 6) Transposta               |");
		printf("\n\t\t\t| 7) Exibir Matriz            |");
		printf("\n\t\t\t|_____________________________|\n\n");
		scanf("%d",&selecionado);
			printf(" \n\n \t >>>>>>>>>>> Opcao: %d",selecionado);
			if(selecionado==0){
				return 0;
			}else if(selecionado==1){
				addMatriz();
				printf(" \n\n \t >>>>>>>>>>>OK");
			}else if(selecionado==2){
				printf("\n\n \t \t >>> Qual o numero da sua Primeira matriz?\n <<<");
				scanf("%d",&mSelected);
				printf("\n\n \t \t >>> Qual o numero da sua Segunda matriz?\n <<<");
				scanf("%d",&mSelected2);
				somarMatriz(mSelected,mSelected2);
				printf(" \n\n \t >>>>>>>>>>>OK");
			}else if(selecionado==3){
				printf("\n\n \t \t >>> Qual o numero da sua Primeira matriz?\n <<<");
				scanf("%d",&mSelected);
				printf("\n\n \t \t >>> Qual o numero da sua Segunda matriz?\n <<<");
				scanf("%d",&mSelected2);
				multpMatriz(mSelected,mSelected2);
			}else if(selecionado==4){
				int nSelect=0;
				printf("\n\n Qual numero se deseja o fatorial? ");
				scanf("%d",&nSelect);
				printf("\n\n\t O fatorial e : %d",fatorial(nSelect));
			}else if(selecionado==5){
				printf("\n\n \t \t >>> Qual o numero da sua matriz?\n <<<");
				scanf("%d",&mSelected);
				printf("\n\n\t A Determinante e : %d",detMatriz(mSelected));
			}else if(selecionado==6){
				printf("\n\n \t \t >>> Qual o numero da sua matriz?\n <<<");
				scanf("%d",&mSelected);
				transpMatriz(mSelected);
			}else if(selecionado==7){
				printf("\n\n \t \t >>> Qual o numero da sua matriz?\n <<<");
				scanf("%d",&mSelected);
				exibir(mSelected);
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