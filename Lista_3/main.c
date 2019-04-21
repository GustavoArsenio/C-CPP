#include <stdlib.h>
#include <stdio.h>
#include "lista.h"

struct topoLista *p,*q,*r;

int main(int argc, char const *argv[])
{
	// Ex1
	p = initTopo(p);
	printf("\n Digite o valor do novo item: " );
	int valor;
	scanf("%d",&valor);
	empilha(valor,p);
	show(p->inicio);
	empilha(topo(p)->valor,p);
	printf("\n Digite o valor do novo item: " );
	scanf("%d",&valor);
	empilha(valor,p);
	show(p->inicio);
	printf(">>> desempilha %d\n ",desempilha(p)->valor);
	show(p->inicio);
	finalizarLista(p->inicio);
	/*
	Ex2
		p = initTopo(p);
		q = initTopo(q);
		r = initTopo(r);
		empilha(1,p);
		empilha(2,p);
		empilha(3,p);
		empilha(4,p);
		empilha(desempilha(p)->valor,q);
		empilha(desempilha(p)->valor,r);
		empilha(desempilha(p)->valor,q);
		empilha(desempilha(p)->valor,r);
		empilha(desempilha(q)->valor,p);
		empilha(desempilha(q)->valor,r);
		empilha(desempilha(p)->valor,r);
		show(r->inicio);
		finalizarLista(p->inicio);
		finalizarLista(q->inicio);
		finalizarLista(r->inicio);
	*/
}