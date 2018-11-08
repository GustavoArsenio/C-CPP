#include <stdio.h>
#include <stdlib.h>
#include "algebra.h"

int main(int argc, char const *argv[])
{
	struct MatrixM Matriz,Matriz2,pMatrizResult,matrizTransp;
	Matriz=buildV(Matriz);
	//system("pause");
	Matriz.matrizTransp = transpose(Matriz.Matriz,Matriz.x,Matriz.y);
	//system("pause");
	//printf(">>> M1:");
	showV(Matriz.Matriz,Matriz.x,Matriz.y);
	//system("pause");
	//printf(">>> MTransposta:");
	printf("\n\n **************************** ");
	showV(Matriz.matrizTransp,Matriz.y,Matriz.x);
	//system("pause");
	printf("\n\n **************************** ");
	setbuf(stdin,NULL);
	Matriz2 = buildV(Matriz2);
	//showV(Matriz2.Matriz,Matriz2.x,Matriz2.y);
	pMatrizResult=sumMatrix(pMatrizResult,Matriz,Matriz2);
	printf("\n\n\n \t\t\t  >>>>>>>>>>>>>  Resultado <<<<<<<<<<<<<<<<");
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