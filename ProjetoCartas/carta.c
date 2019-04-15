#include "carta.h"
#include<stdlib.h>
#include<string.h>

// funções de matriz
Carta *carta_cria(int id, char num, char *naipe)
{
  Carta *c = (Carta*)malloc(sizeof(Carta));
  strcpy(c->naipe,naipe);
  c->id=id;
  c->num=num;
  return c;
}


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