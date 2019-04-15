#include "carta.h"

#ifndef LISTA_H
#define LISTA_H

// Tipos definidos pelo usuário
typedef struct lista Lista;


// definição da struct lista de matrizes
struct lista{
 int info;	
 Carta *topo;
};

// declarações das funções

// funções de lista
 Lista *lst_cria(void);
// void lst_insere_inicio(List *L, Mat *novo);
void lst_insere_fim(Lista *L, Carta *nova);
// void lst_insere_pos(List *L, Mat *novo);
void lst_exibe_elementos(Lista *L);
// List *lst_busca(List *L, int pos);
// List *lst_busca(List *L, char *nome);
// List *lst_remove_fim(List *L, int pos);
// List *lst_remove_fim(List *L, char *nome);
// List *lst_remove_inicio(List *L, int pos);
// List *lst_remove_inicio(List *L, char *nome);
// List *lst_remove_pos(List *L, int pos);
// List *lst_remove_pos(List *L, char *nome);
void lst_libera(Lista *L);


#endif