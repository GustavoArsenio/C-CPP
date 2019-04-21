#include <stdlib.h>
#include <stdio.h>
#include "lista.h"

void show(struct LISTA *lista){
	if (lista->anterior==NULL)
	{
		printf("Estrutura: \n");
		/* code */
	}
	printf("\t>>> Valor : %d\n", lista->valor);
	if (lista->proximo != NULL)
	{
		show(lista->proximo);
		printf("\n\n");
	}
	
};
struct LISTA *alocar(int tamanho){
	return ( (struct LISTA *) malloc(sizeof(struct LISTA) * tamanho) );
};
void empilha(int valorInput,struct topoLista *varTopo){
	struct LISTA *temporaria =  alocar(1);
	temporaria->valor = valorInput;
	temporaria->proximo = NULL;
	temporaria->anterior=NULL;
	if (varTopo->inicio != NULL)
	{
		temporaria->proximo=alocar(1);
		temporaria->proximo=varTopo->inicio;
		varTopo->inicio->anterior = alocar(1);
		varTopo->inicio->anterior = temporaria;
	}else{
		varTopo->inicio = alocar(1);
	}
	varTopo->inicio = temporaria;
};
struct LISTA *topo(struct topoLista *varTopo){
	return varTopo->inicio;
};
struct LISTA *desempilha(struct topoLista *varTopo){
	struct LISTA *temporaria =  alocar(1);
	temporaria = varTopo->inicio;
	varTopo->inicio = varTopo->inicio->proximo;
	temporaria->proximo->anterior = NULL;
	temporaria->proximo = NULL;
	temporaria->anterior = NULL;
	return temporaria;
};
struct topoLista *initTopo(struct topoLista *p){
	p = (struct topoLista *) malloc(sizeof(struct topoLista));
	p->inicio = NULL;
	return p;
};
void finalizarLista(struct LISTA *lista){
	if (lista->proximo != NULL)
	{
		finalizarLista(lista->proximo);
		free(lista);
	}
}
void calcBin(int valor,int base,struct LISTA *lista){
	if (valor==0)
	{
		return;
	}else if (valor>0)
	{
		empilha(valor%base,lista);
		calcBin((valor-(valor%base)/base),lista)
	}else{
		printf("\n >>> Valor: não é válido %d\n", );
	}
}