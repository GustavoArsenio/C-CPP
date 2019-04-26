#include <stdlib.h>
#include <stdio.h>
#include "lista.h"

struct topoLista *p,*q,*r;
struct topoLista_CHAR *p_CHAR,*q_CHAR;
int main(int argc, char const *argv[])
{
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
}