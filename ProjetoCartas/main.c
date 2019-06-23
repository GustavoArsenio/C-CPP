#include <stdio.h>
#include "deck.h"


int main(int argc, char const *argv[])
{

// ================================== //
//                                    //
//     Setando Variaveis Globais      //
//                                    //
// ================================== //

    struct carta vira;
    int numeroDeCartas, numeroDeNipes = 0, numeroDePlayers = 0;
    struct mao *players;

    printf("\n>>> def Numero de players");
    numeroDePlayers = askPlayers();
    printf("\n>>> def contar 1");
    numeroDeCartas = contar(1);
    printf("\n>>> def contar 2");
    numeroDeNipes = contar(2);
    printf("\n>>> def alocarMaos ");
    players = alocarMaos(1);
    if(numeroDeNipes*numeroDeCartas < numeroDePlayers*3){
        printf("\n Numeros de usuarios invalido, valor muito alto");
        return 1;
    }
    printf("\n>>> distribuindo ");
    players = distribuir(numeroDeCartas,numeroDeNipes,players,vira,numeroDePlayers);
    printf("\n>>> Mostrando ---------- ");
    showPlayers(players,numeroDePlayers);




    // for (int i = 0; i < numeroDePlayers; i++)
    // {
    //     printf("\n\n>>>> Player [%d] \n\n", i + 1);
    //     mostrarMao(players);
    // }
}