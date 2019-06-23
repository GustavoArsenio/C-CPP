#include <stdio.h>
#include "deck.h"

// ================================== //
//                                    //
//     Setando Variaveis Globais      //
//                                    //
// ================================== //
struct carta vira;
int numeroDeCartas, numeroDeNipes, numeroDePlayers;
struct mao *players;

int main(int argc, char const *argv[])
{
    numeroDePlayers = askPlayers();
    players = alocarMaos(numeroDePlayers);
    numeroDeCartas = contar(1);
    numeroDeNipes = contar(2);

    printf(' >>> Numero de nipes : %d', numeroDeNipes);
    printf('\n >>> Numero de cartas : %d', numeroDeCartas);

    for (int i = 0; i < numeroDePlayers; i++)
    {
        printf('\n\n>>>> Player [%d]', i + 1);
        mostrarMao(players[i]);
    }
}