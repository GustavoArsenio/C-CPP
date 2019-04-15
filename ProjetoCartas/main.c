#include <stdio.h>
#include <string.h>
#include "lista.h"
#include "carta.h"
#include "util.h"

int main(int argc, char *argv[]) {
  Carta *C;
  FILE *p;
  Lista *L;
  char nome[20];
  int id=0;
  char num;
  char naipe[10];
  if(argc != 2)
  {
    printf("Digite o nome do arquivo: ");
    scanf("%s",nome);
  }else
  {
    strcpy(nome,argv[1]);
  }

  p= util_abre_arq(nome);
  L = lst_cria();
  do{
    id++;
    fgets(nome,20,p);
    if(!feof(p))
    nome[strlen(nome)-1]='\0';
    else
    nome[strlen(nome)]='\0';
    sscanf(nome,"%c%s",&num,naipe);
    C = carta_cria( id,  num, naipe);
    lst_insere_fim(L, C);

  }while(id<40 && !feof(p));
  
  lst_exibe_elementos(L);
  lst_libera(L);

  return 0;
}