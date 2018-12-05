
/*

###################################
# Construido por: Gustavo Arsenio #
# Curso: Engenharia da computação #
# RA: 0074830                     #
# UNASP Campus São Paulo          #
###################################

*/

#ifndef ALGEBRA_H
#define ALGEBRA_H
struct conjuntoMatrizes{
	struct MatrixM * Matriz;
	struct MatrixM * auxMatriz;
	int tamanho;
};
struct MatrixM{
	char nome[100];
	int description;
	int **matrizTransp;
	int **Matriz;
	int x;
	int y;
};
int fatorial(int i);
int determinante(struct MatrixM Matrix);
void liberaAllM(struct conjuntoMatrizes matrizes);
struct MatrixM buildV(struct MatrixM Matrix);
struct MatrixM red(struct MatrixM Matrix, int x, int y);
void showM(struct MatrixM Matrix);
void soma(int **pMatriz,int x,int y);
struct MatrixM sumMatrix(struct MatrixM pMatrizResult,struct MatrixM pMatriz, struct MatrixM pMatriz2);
struct MatrixM multMatrix(struct MatrixM pMatriz, struct MatrixM pMatriz2);
int **instMatrix(int **pMatriz, int x, int y);
int **transpose(struct MatrixM pMatriz);
void liberaM(int **pMatriz,int x);
struct conjuntoMatrizes addMatriz(struct conjuntoMatrizes Matrizes);
void showMatriz(struct conjuntoMatrizes matrizes,int mSelected);
// int det(int **pMatriz, int l, int c);
// void sisEqn(int vet[][TAM],int v[][TAM], int l, int c);

#endif