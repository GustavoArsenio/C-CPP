#include <stdio.h>
#include <stdlib.h>
#include "deck.h"

int askPlayers()
{
    int numeroDeJogadores = 0;
    while (numeroDeJogadores <= 1)
    {
        printf(" \n Digite a quantidade de jogadores: ");
        scanf("%d", &numeroDeJogadores);
    }
    return numeroDeJogadores;
};

struct mao *alocarMaos(int numeroDeJogadores)
{
    return (struct mao *)malloc(sizeof(struct mao) * numeroDeJogadores);
};

struct mao * distribuir(int numeroDeCartas, int numeroDeNipes,struct mao * players,struct carta vira, int numeroDePlayers){
    printf("\n >>> INICIANDO distribuir");
    struct mao *sorteadas = alocarMaos(numeroDePlayers*3);
    printf("\n >>> INICIANDO 'sorteadas' ");
    sorteadas->proxCarta=NULL;
    sorteadas->cartaAtual.nipe=(-1);
    sorteadas->cartaAtual.valor=(-1);
    for (int i = 0; i < numeroDePlayers; i++)
    {
        printf("\n >>> INICIANDO distribuir player %d",i);
        struct carta sorteada = sortearCarta(numeroDeCartas,numeroDeNipes);
        printf("\n >>> INICIANDO sorteada 1");
        while(verificaSorteado(sorteadas,sorteada) == 1){
            printf("\n >>> oooo");
            sorteada = sortearCarta(numeroDeCartas,numeroDeNipes);
        }
        printf("\n >>> CARTA 1");
        players->cartaAtual = sorteada;
        while(verificaSorteado(sorteadas,sorteada) == 1){
            sorteada = sortearCarta(numeroDeCartas,numeroDeNipes);
        }
        printf("\n >>> CARTA 2");
        players->proxCarta->cartaAtual=sorteada;
        while(verificaSorteado(sorteadas,sorteada) == 1){
            sorteada = sortearCarta(numeroDeCartas,numeroDeNipes);
        }
        printf("\n >>> CARTA 3");
        players->proxCarta->proxCarta->cartaAtual=sorteada;
        players++;
    }
    for (int i = numeroDePlayers; i < 0; i--)
    {
        players--;
    }
    return players;
    
    
}
struct carta sortearCarta(int numeroDeCartas,int numeroDeNipes)
{
    struct carta sorteada;
    sorteada.valor=gerarCartaAleatoria(numeroDeCartas);
    sorteada.nipe = gerarCartaAleatoria(numeroDeNipes);
    return sorteada;
};

int verificaSorteado(struct mao *sorteadas, struct carta sorteada){
    if( (sorteadas->cartaAtual.nipe == sorteada.nipe) && (sorteadas->cartaAtual.valor == sorteada.valor) ){
        return 1;
    }
    if(sorteadas->proxCarta==NULL)
    {
        return 0;
    }
    return verificaSorteado(sorteadas++,sorteada);
    
    
}

int contar(int possibilidade)
{
    // Conta as linhas no arquivo

    printf("\n >>> Definindo qtt");
    int qtt = 0;
    printf("\n >>> Instanciando digitoAux");
    char digitoAuxiliar;
    printf("\n >>> Definindo FILE");
    FILE *arquivo;
    printf("\n >>> Definindo possibilidade");
    if (possibilidade == 1){
        arquivo = fopen("cartas.txt", "r");
        }
    else{
        arquivo = fopen("nipes.txt", "r");
    }
    printf("\n >>> Atribuindo digitoAux ");
    digitoAuxiliar = getc(arquivo);

    while (digitoAuxiliar != EOF)
    {
        printf(" | %c ",digitoAuxiliar);
        if (digitoAuxiliar == '\n'){
            printf("\n >>> incrementando 1");
            qtt += 1;
        }
        digitoAuxiliar = getc(arquivo);
    }
    fclose(arquivo);
    printf("\n >>> FIM - QTT: %d",qtt + 1);
    return (qtt + 1);
};

int gerarCartaAleatoria(int LimiteSuperiorDeNumero)
{
    return (rand() % LimiteSuperiorDeNumero + 1);
}
void mostraCarta(struct carta CARTA)
{
    printf("\n%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",201,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,187);
	printf("%c %d               %c\n",186,CARTA.valor,186);
	printf("%c                 %c\n",186,186);
	printf("%c                 %c\n",186,186);
	printf("%c                 %c\n",186,186);
	printf("%c                 %c\n",186,186);
	printf("%c                 %c\n",186,186);
	printf("%c                 %c\n",186,186);
	printf("%c                 %c\n",186,186);
	printf("%c                 %c\n",186,186);
	printf("%c               %d %c\n",186,CARTA.valor,186);
	printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",200,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,188);
}

void showPlayers(struct mao * players, int numeroDePlayers){
    for (int i = 0; i < numeroDePlayers; i++,numeroDePlayers++)
    {
        struct mao * playersAux = players;
        do{
            mostraCarta(playersAux->cartaAtual);
            playersAux=playersAux->proxCarta;
        }
        while ( playersAux->proxCarta != NULL);
        
    }
    
}