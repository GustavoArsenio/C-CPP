#ifndef ALGEBRA_H
#define ALGEBRA_H

int **buildV(int **pMatriz, int x, int y);
void showV(int **pMatrizS, int x, int y);
void soma(int **pMatriz,int x,int y);
int **sumMatrix(int **pMatrizResult,int **pMatriz, int **pMatriz2, int x,int y);
int **popMatrix(int **pMatriz, int x, int y);
int **transpose(int **pMatriz,int x,int y);
int determinante(int **pMatriz,int x,int y);
// int det(int **pMatriz, int l, int c);
// void sisEqn(int vet[][TAM],int v[][TAM], int l, int c);

#endif