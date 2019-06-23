#include <stdio.h>
#include <stdlib.h>
#include "deck.h"

int askPlayers()
{
    int numeroDeJogadores = 0;
    while (numeroDeJogadores > 1)
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

int contar(int possibilidade)
{
    int qtt = 0;

    char digitoAuxiliar;

    FILE *arquivo;

    if (possibilidade == 1)
        arquivo = fopen('C:\\Users\\gusta\\Documents\\Projetos\\C-CPP\\ProjetoCartas\\cartas.txt', 'r');
    else
        arquivo = fopen('C:\\Users\\gusta\\Documents\\Projetos\\C-CPP\\ProjetoCartas\\cartas.txt', 'r');

    digitoAuxiliar = getc(arquivo);

    while (digitoAuxiliar != EOF)
    {
        if (digitoAuxiliar == '\n')
            qtt += 1;
    }

    fclose(arquivo);

    return qtt + 1;
};

int gerarNumeroAleatorio(int LimiteSuperiorDeNumero)
{
    return (rand() % LimiteSuperiorDeNumero + 1);
}

void mostrarMao(struct mao *MAO)
{
    while (MAO->proxCarta != NULL)
    {
        mostraCarta(MAO->cartaAtual);
        MAO = MAO->proxCarta;
    }
    mostraCarta(MAO->cartaAtual);
}

void mostraCarta(struct carta CARTA)
{
}