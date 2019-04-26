#include <stdlib.h>
#include <stdio.h>
#include "lista.h"

struct topoLista *p,*q,*r;
struct topoLista_CHAR *p_CHAR,*q_CHAR;
int main(int argc, char const *argv[])
{
	p_CHAR = initTopo_CHAR(p_CHAR);
	char entrada ='a';
	printf("\n Digite a string desejada: ");
	while(1==1){
		entrada=getc(stdin);
		if (entrada=='\n')	
		{
			break;

		}else{
			empilha_CHAR(entrada,p_CHAR);
		};
	};
	if(comparar_CHAR(p_CHAR->inicio,p_CHAR->fim) == 0){
		printf("\n *** E um palindromo *** \n");
	}else{
		printf("\n *** Nao e um palindromo *** \n");
	}
}