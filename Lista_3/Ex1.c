#include <stdlib.h>
#include <stdio.h>
#include "lista.h"

struct topoLista *p,*q,*r;
struct topoLista_CHAR *p_CHAR,*q_CHAR;
int main(int argc, char const *argv[])
{
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
}