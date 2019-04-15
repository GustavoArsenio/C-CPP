#ifndef CARTA_H
#define CARTA_H

// Tipos definidos pelo usuário
typedef struct carta Carta;


// definição da struct matriz
struct carta{
	int id;
	char num;
	char naipe[10];
	struct carta *prox;
};


// declarações das funções


// funções de matriz
 Carta *carta_cria(int id, char num, char *naipe);
// Mat *mat_cria2(int linha, int coluna, char *nome);
// void mat_carrega(Mat *M);
// void mat_exibe(Mat *M);
// Mat *mat_soma(Mat *A, Mat *B);
// Mat *mat_mult(Mat *A, Mat *B);
// Mat *mat_transposta(Mat *M);
// float mat_determinante(Mat *M);
// void libera(List *L);
// Mat* mat_read_arq(char *nome);
// void mat_write_arq(Mat *M);

#endif