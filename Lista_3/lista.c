#include <stdlib.h>
#include <stdio.h>
#include "lista.h"

void show(struct LISTA *lista){
	if (lista->anterior==NULL)
	{
		printf("Estrutura: \n");
		/* code */
	}
	printf("%d\n", lista->valor);
	if (lista->proximo != NULL)
	{
		show(lista->proximo);
		printf("\n\n");
	}
};
void show_CHAR(struct LISTA_CHAR *lista){
	if (lista->anterior==NULL)
	{
		printf("Invertido: \n");
		/* code */
	}
	printf("%c", lista->valor);
	if (lista->proximo != NULL)
	{
		show_CHAR(lista->proximo);
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
};
void calcBin(int valor,int base,struct topoLista *topo){
	if (valor==0)
	{
			return;
	}else if (valor>0)
	{
		printf("\n%d",valor%base);
		empilha(valor%base,topo);
		calcBin(((valor-(valor%base))/base),base,topo);
	}else{
		printf("\n >>> Valor: não é válido %d\n", valor);
	}
};
struct LISTA_CHAR *alocar_CHAR(int tamanho){
	return ( (struct LISTA_CHAR *) malloc(sizeof(struct LISTA_CHAR) * tamanho) );
};
void empilha_CHAR(char valorInput,struct topoLista_CHAR *varTopo){
	struct LISTA_CHAR *temporaria =  alocar_CHAR(1);
	temporaria->valor = valorInput;
	temporaria->proximo = NULL;
	temporaria->anterior=NULL;
	if (varTopo->inicio != NULL)
	{
		temporaria->proximo=alocar_CHAR(1);
		temporaria->proximo=varTopo->inicio;
		varTopo->inicio->anterior = alocar_CHAR(1);
		varTopo->inicio->anterior = temporaria;
	}else{
		varTopo->inicio = alocar_CHAR(1);
		varTopo->fim = alocar_CHAR(1);
		varTopo->fim = temporaria;
	}
	varTopo->inicio = temporaria;
};
struct LISTA_CHAR *desempilha_CHAR(struct topoLista_CHAR *varTopo){
	struct LISTA_CHAR *temporaria =  alocar_CHAR(1);
	temporaria = varTopo->inicio;
	varTopo->inicio = varTopo->inicio->proximo;
	temporaria->proximo->anterior = NULL;
	temporaria->proximo = NULL;
	temporaria->anterior = NULL;
	return temporaria;
};
struct topoLista_CHAR *initTopo_CHAR(struct topoLista_CHAR *p){
	p = (struct topoLista_CHAR *) malloc(sizeof(struct topoLista_CHAR));
	p->inicio = NULL;
	return p;
};
void finalizarLista_CHAR(struct LISTA_CHAR *lista){
	if (lista->proximo != NULL)
	{
		finalizarLista_CHAR(lista->proximo);
		free(lista);
	}
};
int comparar_CHAR(struct LISTA_CHAR *lista1,struct LISTA_CHAR *lista2){
	if (lista1->valor == lista2->valor)
	{
		if (lista1->proximo == NULL && lista2->anterior == NULL)
		{
			return 0;
		}else if(lista1->proximo == NULL || lista2->anterior == NULL){
			return 1;
		}else{
			return comparar_CHAR(lista1->proximo,lista2->anterior);
		};
	}else{
		return 1;
	}
	return 1;
};