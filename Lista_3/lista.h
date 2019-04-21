#ifndef LISTA_H
#define LISTA_H

struct LISTA{
	struct LISTA *anterior;
	int valor;
	struct LISTA *proximo;
};
struct topoLista{
	struct LISTA *inicio;
};
void show(struct LISTA *lista);
struct LISTA *alocar(int tamanho);
void empilha(int valorInput,struct topoLista *varTopo);
struct LISTA *topo(struct topoLista *varTopo);
struct LISTA *desempilha(struct topoLista *varTopo);
struct topoLista *initTopo(struct topoLista *p);
void finalizarLista(struct LISTA *lista);
void calcBin(int valor,int base,struct LISTA *lista);
#endif