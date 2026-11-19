#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "truco.h"

enum { PLAYER_COUNT = 4, CARDS_PER_HAND = 3 };

static void destroy_state(struct truco_card_list *deck,
                          struct truco_card_list *hands[PLAYER_COUNT],
                          struct truco_card_list *face_up) {
    for (size_t player = 0; player < PLAYER_COUNT; ++player) {
        truco_card_list_destroy(hands[player]);
    }
    truco_card_list_destroy(face_up);
    truco_card_list_destroy(deck);
}

static void print_card(struct truco_card card) {
    printf("%s of %s", truco_rank_name(card.rank), truco_suit_name(card.suit));
}

static bool read_seed(uint32_t *seed) {
    uint64_t input;
    if (scanf("%" SCNu64, &input) != 1 || input > UINT32_MAX) {
        return false;
    }
    *seed = (uint32_t)input;
    return true;
}

int main(void) {
    uint32_t seed;
    if (!read_seed(&seed)) {
        fputs("Expected a seed from 0 through 4294967295.\n", stderr);
        return 1;
    }

    struct truco_card_list *deck = truco_standard_deck_create();
    struct truco_card_list *hands[PLAYER_COUNT] = {0};
    struct truco_card_list *face_up = truco_card_list_create();
    if (deck == NULL || face_up == NULL) {
        fputs("Unable to allocate the deck.\n", stderr);
        destroy_state(deck, hands, face_up);
        return 1;
    }
    for (size_t player = 0; player < PLAYER_COUNT; ++player) {
        hands[player] = truco_card_list_create();
        if (hands[player] == NULL) {
            fputs("Unable to allocate a player hand.\n", stderr);
            destroy_state(deck, hands, face_up);
            return 1;
        }
    }

    if (!truco_deck_shuffle(deck, seed)) {
        destroy_state(deck, hands, face_up);
        return 1;
    }
    for (size_t round = 0; round < CARDS_PER_HAND; ++round) {
        for (size_t player = 0; player < PLAYER_COUNT; ++player) {
            struct truco_card card;
            if (!truco_card_list_pop_front(deck, &card) ||
                !truco_card_list_append(hands[player], card)) {
                fputs("Unable to deal the cards.\n", stderr);
                destroy_state(deck, hands, face_up);
                return 1;
            }
        }
    }
    struct truco_card vira;
    if (!truco_card_list_pop_front(deck, &vira) ||
        !truco_card_list_append(face_up, vira)) {
        fputs("Unable to reveal the vira.\n", stderr);
        destroy_state(deck, hands, face_up);
        return 1;
    }

    printf("Seed: %" PRIu32 "\nVira: ", seed);
    print_card(vira);
    printf("\nManilhas: %s (clubs > hearts > spades > diamonds)\n",
           truco_rank_name(truco_manilha_rank(vira.rank)));
    for (size_t player = 0; player < PLAYER_COUNT; ++player) {
        printf("Player %zu:", player + 1);
        for (size_t card_index = 0;
             card_index < truco_card_list_size(hands[player]); ++card_index) {
            struct truco_card card;
            if (!truco_card_list_get(hands[player], card_index, &card)) {
                destroy_state(deck, hands, face_up);
                return 1;
            }
            printf(" ");
            print_card(card);
        }
        putchar('\n');
    }
    printf("Undealt cards: %zu\n", truco_card_list_size(deck));

    destroy_state(deck, hands, face_up);
    return 0;
}
