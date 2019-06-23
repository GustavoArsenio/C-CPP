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
    struct mao *proxCarta;
    struct mao *proxMao;
};

int contar(int possibilidade);
int askPlayers();
struct mao *alocarMaos(int numeroDeJogadores);
int gerarNumeroAleatorio(int LimiteSuperiorDeNumero);
void mostrarMao(struct mao *MAO);
void mostraCarta(struct carta CARTA);
int gerarCartaAleatoria(int LimiteSuperiorDeNumero);
struct mao * distribuir(int numeroDeCartas, int numeroDeNipes,struct mao * players,struct carta vira,int numeroDePlayers);
void showPlayers(struct mao * players,int numeroDePlayers);
struct carta sortearCarta(int numeroDeCartas,int numeroDeNipes);
int verificaSorteado(struct mao *sorteadas, struct carta sorteada);
#endif 