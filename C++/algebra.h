#ifndef ALGEBRA_H
#define ALGEBRA_H
struct MatrixM{
	char nome[15];
	int **matrizTransp;
	int **Matriz;
	int x;
	int y;
};

struct MatrixM buildV(struct MatrixM Matrix);
void showV(int **pMatrizS, int x, int y);
void soma(int **pMatriz,int x,int y);
struct MatrixM sumMatrix(struct MatrixM pMatrizResult,struct MatrixM pMatriz, struct MatrixM pMatriz2);
int **multMatrix(struct MatrixM Matriz, struct MatrixM pMatriz2);
int **instMatrix(int **pMatriz, int x, int y);
int **transpose(int **pMatriz,int x,int y);
void liberaM(int **pMatriz,int x);
// int det(int **pMatriz, int l, int c);
// void sisEqn(int vet[][TAM],int v[][TAM], int l, int c);

#endif