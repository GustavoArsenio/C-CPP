#include "card_renderer.h"

#include <string.h>

bool truco_render_card(struct truco_card card,
                       char output[TRUCO_RENDERED_CARD_TEXT_SIZE]) {
    static const char ranks[] = "4567QJKA23";
    static const char suits[] = "DSHC";
    static const char top[] = "+-----------------+\n";
    static const char blank[] = "|                 |\n";
    char rendered[TRUCO_RENDERED_CARD_TEXT_SIZE];
    size_t offset = 0;

    if (output == NULL || card.rank < TRUCO_FOUR || card.rank > TRUCO_THREE ||
        card.suit < TRUCO_DIAMONDS || card.suit > TRUCO_CLUBS) {
        return false;
    }

    memcpy(rendered + offset, top, sizeof(top) - 1);
    offset += sizeof(top) - 1;
    rendered[offset++] = '|';
    rendered[offset++] = ' ';
    rendered[offset++] = ranks[(int)card.rank];
    memset(rendered + offset, ' ', 13);
    offset += 13;
    rendered[offset++] = suits[(int)card.suit];
    rendered[offset++] = ' ';
    rendered[offset++] = '|';
    rendered[offset++] = '\n';
    for (int row = 0; row < 8; ++row) {
        memcpy(rendered + offset, blank, sizeof(blank) - 1);
        offset += sizeof(blank) - 1;
    }
    rendered[offset++] = '|';
    rendered[offset++] = ' ';
    rendered[offset++] = suits[(int)card.suit];
    memset(rendered + offset, ' ', 13);
    offset += 13;
    rendered[offset++] = ranks[(int)card.rank];
    rendered[offset++] = ' ';
    rendered[offset++] = '|';
    rendered[offset++] = '\n';
    memcpy(rendered + offset, top, sizeof(top) - 1);
    offset += sizeof(top) - 1;
    rendered[offset] = '\0';

    memcpy(output, rendered, offset + 1);
    return true;
}
