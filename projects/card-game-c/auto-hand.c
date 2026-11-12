#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "truco.h"

static bool deal_hand(struct truco_card_list *deck,
                      struct truco_card_list *hands[TRUCO_PLAYER_COUNT],
                      struct truco_card *vira) {
    for (size_t card = 0; card < 3; ++card) {
        for (size_t player = 0; player < TRUCO_PLAYER_COUNT; ++player) {
            struct truco_card dealt;
            if (!truco_card_list_pop_front(deck, &dealt) ||
                !truco_card_list_append(hands[player], dealt)) {
                return false;
            }
        }
    }
    return truco_card_list_pop_front(deck, vira);
}

static bool play_lowest_card(struct truco_hand *hand,
                             struct truco_card_list *hands[TRUCO_PLAYER_COUNT],
                             enum truco_rank manilha) {
    const size_t player = truco_hand_current_player(hand);
    if (player >= TRUCO_PLAYER_COUNT ||
        truco_card_list_size(hands[player]) == 0) {
        return false;
    }
    struct truco_card lowest;
    if (!truco_card_list_get(hands[player], 0, &lowest)) {
        return false;
    }
    for (size_t index = 1; index < truco_card_list_size(hands[player]);
         ++index) {
        struct truco_card candidate;
        if (!truco_card_list_get(hands[player], index, &candidate)) {
            return false;
        }
        if (truco_card_compare(candidate, lowest, manilha) < 0) {
            lowest = candidate;
        }
    }
    return truco_hand_play(hand, lowest);
}

int main(int argc, char **argv) {
    if (argc > 2) {
        fprintf(stderr, "Usage: auto-hand <seed>\n");
        return 2;
    }
    char *end = NULL;
    uintmax_t parsed_seed;
    if (argc == 2) {
        parsed_seed = strtoumax(argv[1], &end, 10);
        if (end == argv[1] || *end != '\0') {
            fprintf(stderr, "Seed must be an unsigned 32-bit integer.\n");
            return 2;
        }
    } else if (scanf("%" SCNuMAX, &parsed_seed) != 1) {
        fprintf(stderr, "Provide an unsigned 32-bit seed.\n");
        return 2;
    }
    if (parsed_seed > UINT32_MAX) {
        fprintf(stderr, "Seed must be an unsigned 32-bit integer.\n");
        return 2;
    }

    struct truco_card_list *deck = truco_standard_deck_create();
    struct truco_card_list *hands[TRUCO_PLAYER_COUNT] = {NULL};
    struct truco_match *match = truco_match_create();
    struct truco_hand *hand = NULL;
    int status = 1;
    if (deck == NULL || match == NULL ||
        !truco_deck_shuffle(deck, (uint32_t)parsed_seed)) {
        goto cleanup;
    }
    for (size_t player = 0; player < TRUCO_PLAYER_COUNT; ++player) {
        hands[player] = truco_card_list_create();
        if (hands[player] == NULL) {
            goto cleanup;
        }
    }
    struct truco_card vira;
    if (!deal_hand(deck, hands, &vira) ||
        truco_match_start_hand(match, TRUCO_HAND_ELEVEN_NONE) !=
            TRUCO_HAND_STARTED) {
        goto cleanup;
    }
    const enum truco_rank manilha = truco_manilha_rank(vira.rank);
    hand = truco_hand_create(hands, match, manilha, 0);
    if (hand == NULL) {
        goto cleanup;
    }
    printf("seed=%" PRIuMAX " vira=%s/%s manilha=%s\n", parsed_seed,
           truco_rank_name(vira.rank), truco_suit_name(vira.suit),
           truco_rank_name(manilha));
    printf(
        "Policy: each player plays their weakest card; no raises; leader 0 "
        "starts every trick.\n");
    while (!truco_hand_is_complete(hand)) {
        if (!play_lowest_card(hand, hands, manilha)) {
            fprintf(stderr, "Automatic hand stopped on an invalid play.\n");
            goto cleanup;
        }
    }
    printf("winner_team=%d score=%d-%d stake=1\n", truco_hand_winner(hand),
           truco_match_score(match, TRUCO_TEAM_ZERO),
           truco_match_score(match, TRUCO_TEAM_ONE));
    status = 0;

cleanup:
    truco_hand_destroy(hand);
    for (size_t player = 0; player < TRUCO_PLAYER_COUNT; ++player) {
        truco_card_list_destroy(hands[player]);
    }
    truco_match_destroy(match);
    truco_card_list_destroy(deck);
    return status;
}
