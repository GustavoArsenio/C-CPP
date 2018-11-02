#ifndef ALGEBRA_H
#define ALGEBRA_H
struct MatrixM{
	char nome[10];
	int **Matriz;
	int x;
	int y;
}

int **buildV(int **pMatriz, int x, int y);
void showV(int **pMatrizS, int x, int y);
void soma(int **pMatriz,int x,int y);
int **sumMatrix(int **pMatrizResult,int **pMatriz, int **pMatriz2, int x,int y);
int **popMatrix(int **pMatriz, int x, int y);
int **transpose(int **pMatriz,int x,int y);
void liberaM(int **pMatriz,int x);
// int det(int **pMatriz, int l, int c);
// void sisEqn(int vet[][TAM],int v[][TAM], int l, int c);

#endif