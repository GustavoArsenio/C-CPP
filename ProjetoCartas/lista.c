#include "lista.h"
#include "carta.h"
#include <stdlib.h>
#include <stdio.h>


// funções de lista
Lista *lst_cria(void){
  Lista *l;
  l = (Lista*)malloc(sizeof(Lista));
  l->topo=NULL;
  l->info=0;
  return l;
}


// void lst_insere_inicio(List *L, Mat *novo);
 void lst_insere_fim(Lista *L, Carta *nova)
 {
   nova->prox = L->topo;
   L->topo=nova;
 }
// void lst_insere_pos(List *L, Mat *novo);
void lst_exibe_elementos(Lista *L)
{
  Carta *ptr;
  for(ptr=L->topo; ptr!=NULL;ptr=ptr->prox)
  {
    printf("Nº %2d: Carta: %c  Naipe: %s \n",ptr->id,ptr->num, ptr->naipe);
  }
}
// List *lst_busca(List *L, int pos);
// List *lst_busca(List *L, char *nome);
// List *lst_remove_fim(List *L, int pos);
// List *lst_remove_fim(List *L, char *nome);
// List *lst_remove_inicio(List *L, int pos);
// List *lst_remove_inicio(List *L, char *nome);
// List *lst_remove_pos(List *L, int pos);
// List *lst_remove_pos(List *L, char *nome);
void lst_libera(Lista *L)
{
  Carta *p;
  for( ; L->topo!=NULL; )
  {
    p = L->topo;
    L->topo = p->prox;
    free(p);
  }
  L->topo=NULL;
  free(L);
}
