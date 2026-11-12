#ifndef C_CPP_CARD_RENDERER_H
#define C_CPP_CARD_RENDERER_H

#include "truco.h"

/* 240 rendered bytes: 12 rows of 19 columns, each followed by '\n', plus the
 * terminating NUL. The caller therefore provides 241 bytes.
 */
enum { TRUCO_RENDERED_CARD_TEXT_SIZE = 12 * (19 + 1) + 1 };

/* Render a card using printable ASCII. Invalid enum values or a null output
 * return false and leave output unchanged. output must point to at least
 * TRUCO_RENDERED_CARD_TEXT_SIZE bytes.
 */
bool truco_render_card(struct truco_card card,
                       char output[TRUCO_RENDERED_CARD_TEXT_SIZE]);

#endif
