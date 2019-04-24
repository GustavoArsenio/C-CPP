#include <stdlib.h>
#include <stdio.h>
#include "lista.h"

struct topoLista *p,*q,*r;
struct topoLista_CHAR *p_CHAR,*q_CHAR;

int main(int argc, char const *argv[])
{
	/* Ex1
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
	*/
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
	/* Ex 3/4
		int base,valor;
		p = initTopo(p);
		printf("\n Digite o valor de base: ");
		scanf("%d",&base);
		printf("\n\n Digite o valor numerico de calculo: ");
		scanf("%d",&valor);
		calcBin(valor,base,p);
		printf("\n Abacou o calculo --- \n");
		show(p->inicio);
	*/
	/*
	//Ex5
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
	*/
	//Ex6
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