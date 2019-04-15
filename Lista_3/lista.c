#include <stdlib.h>
#include <stdio.h>
#include "lista.h"

void show(struct LISTA *lista){
	printf(">>> Valor : %d\n", lista->valor);
	if (lista->proximo != NULL)
	{
		show(lista->proximo);
	}
	
};
struct LISTA *alocar(int tamanho){
	return ( (struct LISTA *) malloc(sizeof(struct LISTA) * tamanho) );
};
void empilha(int valorInput,struct topoLista *topo){
	struct LISTA *temporaria =  alocar(1);
	temporaria->proximo=alocar(1);
	temporaria->anterior=alocar(1);
	temporaria->valor = valorInput;
	temporaria->proximo=topo->inicio;
	printf("\n%d\n",temporaria->valor);
	if (topo->inicio != NULL)
	{
		topo->inicio->anterior = temporaria;
	}else{
		temporaria->anterior = NULL;
		topo->inicio = alocar(1);
	}
	topo->inicio = temporaria;
};
struct LISTA *top(struct topoLista *topo){
	return topo->inicio;
};
struct LISTA *desempilha(struct topoLista *topo){
	struct LISTA *temporaria =  alocar(1);
	temporaria = topo->inicio;
	topo->inicio = topo->inicio->proximo;
	temporaria->proximo = NULL;
	temporaria->anterior = NULL;
	return temporaria;
};