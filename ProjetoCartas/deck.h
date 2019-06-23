#ifndef DECK_H
#define DECK_H

struct carta
{
    int valor;
    int nipe;
};

struct mao
{
    struct carta cartaAtual;
    struct carta *proxCarta;
};

int contar(int possibilidade);
int askPlayers();
struct mao *alocarMaos(int numeroDeJogadores);
int gerarNumeroAleatorio(int LimiteSuperiorDeNumero);
void mostrarMao(struct mao MAO);

#endif DECK_H