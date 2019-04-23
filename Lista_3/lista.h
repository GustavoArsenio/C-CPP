#ifndef LISTA_H
#define LISTA_H

struct LISTA{
	struct LISTA *anterior;
	int valor;
	struct LISTA *proximo;
};
struct LISTA_CHAR{
	struct LISTA *anterior;
	char valor;
	struct LISTA *proximo;
};
struct topoLista{
	struct LISTA *inicio;
};
void show(struct LISTA *lista);
struct LISTA_CHAR *alocar(int tamanho);
void empilha(int valorInput,struct topoLista *varTopo);
struct LISTA_CHAR *topo(struct topoLista *varTopo);
struct LISTA_CHAR *desempilha(struct topoLista *varTopo);
struct topoLista *initTopo(struct topoLista *p);
void finalizarLista(struct LISTA *lista);
void calcBin(int valor,int base,struct topoLista *topo);
void show(struct LISTA *lista);
struct LISTA_CHAR *alocar(int tamanho);
void empilha(int valorInput,struct topoLista *varTopo);
struct LISTA_CHAR *topo(struct topoLista *varTopo);
struct LISTA_CHAR *desempilha(struct topoLista *varTopo);
struct topoLista *initTopo(struct topoLista *p);
void finalizarLista(struct LISTA *lista);
#endif