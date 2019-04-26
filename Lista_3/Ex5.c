#include <stdlib.h>
#include <stdio.h>
#include "lista.h"

struct topoLista *p,*q,*r;
struct topoLista_CHAR *p_CHAR,*q_CHAR;
int main(int argc, char const *argv[])
{
	p_CHAR = initTopo_CHAR(p_CHAR);
	char entrada ='a';
	printf("\n Digite a string desejada para inverter:\n ");
	while(1==1){
		entrada=getc(stdin);
		if (entrada=='\n')	
		{
			break;

		}else{
			empilha_CHAR(entrada,p_CHAR);
		}
	}
	show_CHAR(p_CHAR->inicio);
}