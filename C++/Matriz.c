
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
	printf("\n\n >>> Terminando Program Matrix: ");
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