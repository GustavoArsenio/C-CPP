#include <stdlib.h>
#include <stdio.h>
#include "lista.h"

struct topoLista *p;

int main(int argc, char const *argv[])
{
	p = (struct topoLista *) malloc(sizeof(struct topoLista));
	printf("\n Digite o valor do novo item: " );
	int valor;
	scanf("%d",&valor);
	empilha(valor,p);
	empilha(top(p)->valor,p);
	printf("\n Digite o valor do novo item: " );
	scanf("%d",&valor);
	empilha(valor,p);
	show(p->inicio);
	printf(">>> desempilha %d\n Estrutura: \n",desempilha(p)->valor );
	show(p->inicio);
}