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
void empilha(int valorInput,struct topoLista *topo);
struct LISTA *desempilha(struct topoLista *topo);
struct LISTA *top(struct topoLista *topo);
struct LISTA *alocar(int tamanho);
#endif