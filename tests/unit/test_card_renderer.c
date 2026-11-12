#include <stdio.h>
#include <string.h>

#include "card_renderer.h"

#define CHECK(condition)                                                       \
    do {                                                                       \
        if (!(condition)) {                                                    \
            fprintf(stderr, "CHECK failed at %s:%d: %s\n", __FILE__, __LINE__, \
                    #condition);                                               \
            return 1;                                                          \
        }                                                                      \
    } while (0)

static int check_render(struct truco_card card, char expected_rank,
                        char expected_suit) {
    static const char top[] = "+-----------------+\n";
    static const char blank[] = "|                 |\n";
    char output[TRUCO_RENDERED_CARD_TEXT_SIZE];
    CHECK(truco_render_card(card, output));
    CHECK(memcmp(output, top, sizeof(top) - 1) == 0);
    CHECK(memcmp(output + 20, "| ", 2) == 0);
    CHECK(output[22] == expected_rank);
    for (size_t index = 23; index < 36; ++index) {
        CHECK(output[index] == ' ');
    }
    CHECK(output[36] == expected_suit);
    CHECK(memcmp(output + 37, " |\n", 3) == 0);
    for (size_t row = 0; row < 8; ++row) {
        CHECK(memcmp(output + 40 + row * 20, blank, sizeof(blank) - 1) == 0);
    }
    CHECK(memcmp(output + 200, "| ", 2) == 0);
    CHECK(output[202] == expected_suit);
    for (size_t index = 203; index < 216; ++index) {
        CHECK(output[index] == ' ');
    }
    CHECK(output[216] == expected_rank);
    CHECK(memcmp(output + 217, " |\n", 3) == 0);
    CHECK(memcmp(output + 220, top, sizeof(top) - 1) == 0);

    const char *terminator = memchr(output, '\0', sizeof(output));
    CHECK(terminator != NULL);
    size_t text_length = (size_t)(terminator - output);
    size_t row_start = 0;
    size_t rows = 0;
    for (size_t index = 0; index < text_length; ++index) {
        if (output[index] == '\n') {
            CHECK(index - row_start == 19);
            row_start = index + 1;
            ++rows;
        }
    }
    CHECK(rows == 12);
    CHECK(row_start == text_length);
    CHECK(text_length == 240);
    CHECK(output[text_length] == '\0');
    return 0;
}

int main(void) {
    static const char golden[] =
        "+-----------------+\n"
        "| A             C |\n"
        "|                 |\n"
        "|                 |\n"
        "|                 |\n"
        "|                 |\n"
        "|                 |\n"
        "|                 |\n"
        "|                 |\n"
        "|                 |\n"
        "| C             A |\n"
        "+-----------------+\n";
    char output[TRUCO_RENDERED_CARD_TEXT_SIZE];
    static const char ranks[] = "4567QJKA23";
    static const char suits[] = "DSHC";

    CHECK(TRUCO_RENDERED_CARD_TEXT_SIZE == 241);
    CHECK(
        truco_render_card((struct truco_card){TRUCO_ACE, TRUCO_CLUBS}, output));
    CHECK(strcmp(output, golden) == 0);
    for (int rank = TRUCO_FOUR; rank <= TRUCO_THREE; ++rank) {
        for (int suit = TRUCO_DIAMONDS; suit <= TRUCO_CLUBS; ++suit) {
            CHECK(check_render((struct truco_card){(enum truco_rank)rank,
                                                   (enum truco_suit)suit},
                               ranks[rank], suits[suit]) == 0);
        }
    }

    memset(output, 'X', sizeof(output));
    CHECK(!truco_render_card(
        (struct truco_card){(enum truco_rank) - 1, TRUCO_DIAMONDS}, output));
    for (size_t index = 0; index < sizeof(output); ++index) {
        CHECK(output[index] == 'X');
    }
    CHECK(!truco_render_card(
        (struct truco_card){(enum truco_rank)10, TRUCO_DIAMONDS}, output));
    CHECK(!truco_render_card(
        (struct truco_card){TRUCO_FOUR, (enum truco_suit) - 1}, output));
    CHECK(!truco_render_card(
        (struct truco_card){TRUCO_FOUR, (enum truco_suit)4}, output));
    CHECK(
        !truco_render_card((struct truco_card){TRUCO_ACE, TRUCO_CLUBS}, NULL));
    for (size_t index = 0; index < sizeof(output); ++index) {
        CHECK(output[index] == 'X');
    }
    return 0;
}
