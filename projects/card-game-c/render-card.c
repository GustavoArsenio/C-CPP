#include <stdio.h>

#include "card_renderer.h"

static bool parse_rank(char symbol, enum truco_rank *rank) {
    static const char symbols[] = "4567QJKA23";
    for (int index = 0; index < 10; ++index) {
        if (symbols[index] == symbol) {
            *rank = (enum truco_rank)index;
            return true;
        }
    }
    return false;
}

static bool parse_suit(char symbol, enum truco_suit *suit) {
    static const char symbols[] = "DSHC";
    for (int index = 0; index < 4; ++index) {
        if (symbols[index] == symbol) {
            *suit = (enum truco_suit)index;
            return true;
        }
    }
    return false;
}

int main(void) {
    char rank_symbol;
    char suit_symbol;
    char extra;
    enum truco_rank rank;
    enum truco_suit suit;
    char output[TRUCO_RENDERED_CARD_TEXT_SIZE];

    if (scanf(" %c %c", &rank_symbol, &suit_symbol) != 2 ||
        !parse_rank(rank_symbol, &rank) || !parse_suit(suit_symbol, &suit)) {
        fputs("Expected a valid rank and suit symbol (for example: A C).\n",
              stderr);
        return 1;
    }
    if (scanf(" %c", &extra) == 1) {
        fputs("Expected exactly two symbols: rank followed by suit.\n", stderr);
        return 1;
    }

    if (!truco_render_card((struct truco_card){rank, suit}, output)) {
        fputs("Could not render card.\n", stderr);
        return 1;
    }
    fputs(output, stdout);
    return 0;
}
