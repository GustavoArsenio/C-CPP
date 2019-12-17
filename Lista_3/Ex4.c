#include <stdlib.h>
#include <stdio.h>
#include "lista.h"

struct topoLista *p,*q,*r;
struct topoLista_CHAR *p_CHAR,*q_CHAR;
int main(int argc, char const *argv[])
{
	int base,valor;
	p = initTopo(p);
	
	printf("\n Digite o valor de base: ");
	scanf("%d",&base);
	
	printf("\n\n Digite o valor numerico de calculo: ");
	scanf("%d",&valor);
	
	calcBin(valor,base,p);
	
	printf("\n --------------------------------------------------- \n");
	
	show(p->inicio);
}