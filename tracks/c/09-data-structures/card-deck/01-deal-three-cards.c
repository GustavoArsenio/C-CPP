#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

enum { CARDS_PER_PLAYER = 3 };

struct card {
    int rank;
    int suit;
};

static bool read_positive_int(int *value) {
    return scanf("%d", value) == 1 && *value > 0;
}

static bool same_card(struct card left, struct card right) {
    return left.rank == right.rank && left.suit == right.suit;
}

static void shuffle(struct card *deck, size_t size) {
    for (size_t index = size; index > 1; --index) {
        size_t other = (size_t)rand() % index;
        struct card temporary = deck[index - 1];
        deck[index - 1] = deck[other];
        deck[other] = temporary;
    }
}

int main(void) {
    int player_count, rank_count, suit_count;
    if (!read_positive_int(&player_count) || !read_positive_int(&rank_count) ||
        !read_positive_int(&suit_count)) {
        fprintf(stderr, "Expected positive player, rank, and suit counts.\n");
        return 1;
    }

    if ((size_t)rank_count > (size_t)-1 / (size_t)suit_count) {
        fprintf(stderr, "Deck dimensions are too large.\n");
        return 1;
    }
    size_t deck_size = (size_t)rank_count * (size_t)suit_count;
    if ((size_t)player_count > (size_t)-1 / CARDS_PER_PLAYER) {
        fprintf(stderr, "Player count is too large.\n");
        return 1;
    }
    size_t hand_size = (size_t)player_count * CARDS_PER_PLAYER;
    if (deck_size < hand_size || deck_size > (size_t)-1 / sizeof(struct card)) {
        fprintf(stderr, "Not enough cards to deal three cards per player.\n");
        return 1;
    }

    struct card *deck = malloc(deck_size * sizeof(*deck));
    if (deck == NULL) {
        fprintf(stderr, "Could not allocate the deck.\n");
        return 1;
    }

    size_t next = 0;
    for (int suit = 1; suit <= suit_count; ++suit) {
        for (int rank = 1; rank <= rank_count; ++rank) {
            deck[next++] = (struct card){.rank = rank, .suit = suit};
        }
    }
    srand(1);
    shuffle(deck, deck_size);

    for (int player = 0; player < player_count; ++player) {
        printf("Player %d:", player + 1);
        for (int card = 0; card < CARDS_PER_PLAYER; ++card) {
            struct card dealt =
                deck[(size_t)player * CARDS_PER_PLAYER + (size_t)card];
            printf(" %d/%d", dealt.rank, dealt.suit);
        }
        putchar('\n');
    }

    for (size_t first = 0; first < hand_size; ++first) {
        for (size_t second = first + 1; second < hand_size; ++second) {
            if (same_card(deck[first], deck[second])) {
                free(deck);
                return 2;
            }
        }
    }

    free(deck);
    return 0;
}
