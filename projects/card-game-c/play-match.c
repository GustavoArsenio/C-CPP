#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "truco.h"

static bool read_line(const char *prompt, char *buffer, size_t capacity) {
    printf("%s\n", prompt);
    fflush(stdout);
    if (fgets(buffer, (int)capacity, stdin) == NULL) {
        return false;
    }
    const size_t length = strlen(buffer);
    if (length > 0 && buffer[length - 1] == '\n') {
        buffer[length - 1] = '\0';
    } else if (!feof(stdin)) {
        int character;
        while ((character = getchar()) != '\n' && character != EOF) {
        }
        return false;
    }
    return true;
}

static bool parse_size(const char *text, size_t *value) {
    if (text == NULL || text[0] == '\0' || text[0] == '-') {
        return false;
    }
    char *end = NULL;
    errno = 0;
    const uintmax_t parsed = strtoumax(text, &end, 10);
    if (errno == ERANGE || end == text || *end != '\0' || parsed > SIZE_MAX) {
        return false;
    }
    *value = (size_t)parsed;
    return true;
}

static bool read_answer(const char *prompt, bool *accepted) {
    char answer[32];
    while (read_line(prompt, answer, sizeof(answer))) {
        if (answer[1] == '\0' && (answer[0] == 'y' || answer[0] == 'Y' ||
                                  answer[0] == 'n' || answer[0] == 'N')) {
            *accepted = answer[0] == 'y' || answer[0] == 'Y';
            return true;
        }
        puts("Enter y or n.");
    }
    return false;
}

static bool read_seed(uint32_t *seed) {
    char input[32];
    size_t parsed;
    if (!read_line("Seed (0..4294967295): ", input, sizeof(input)) ||
        !parse_size(input, &parsed) || parsed > UINT32_MAX) {
        return false;
    }
    *seed = (uint32_t)parsed;
    return true;
}

static bool read_point_award(unsigned int *points) {
    char input[32];
    while (read_line("Iron-hand point award (1..12): ", input, sizeof(input))) {
        size_t parsed;
        if (parse_size(input, &parsed) && parsed > 0 && parsed <= 12) {
            *points = (unsigned int)parsed;
            return true;
        }
        puts("Choose an integer from 1 through 12.");
    }
    return false;
}

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

static void destroy_hand_state(
    struct truco_card_list *deck,
    struct truco_card_list *hands[TRUCO_PLAYER_COUNT],
    struct truco_hand *hand) {
    truco_hand_destroy(hand);
    for (size_t player = 0; player < TRUCO_PLAYER_COUNT; ++player) {
        truco_card_list_destroy(hands[player]);
    }
    truco_card_list_destroy(deck);
}

static void show_current_hand(size_t player,
                              const struct truco_card_list *cards) {
    printf("Player %zu (team %zu), cards:", player, player % 2);
    for (size_t index = 0; index < truco_card_list_size(cards); ++index) {
        struct truco_card card;
        if (!truco_card_list_get(cards, index, &card)) {
            return;
        }
        printf(" [%zu] %s/%s", index, truco_rank_name(card.rank),
               truco_suit_name(card.suit));
    }
    putchar('\n');
}

static void show_partner_hand(
    int team, struct truco_card_list *hands[TRUCO_PLAYER_COUNT]) {
    /* Seat parity defines partnerships: 0/2 and 1/3. */
    const size_t partner = team == TRUCO_TEAM_ZERO ? 2 : 3;
    printf("Team %d inspects partner %zu's hand.\n", team, partner);
    show_current_hand(partner, hands[partner]);
}

static bool prepare_deal(uint32_t seed, struct truco_card_list **deck,
                         struct truco_card_list *hands[TRUCO_PLAYER_COUNT],
                         struct truco_card *vira) {
    *deck = truco_standard_deck_create();
    if (*deck == NULL || !truco_deck_shuffle(*deck, seed)) {
        return false;
    }
    for (size_t player = 0; player < TRUCO_PLAYER_COUNT; ++player) {
        hands[player] = truco_card_list_create();
        if (hands[player] == NULL) {
            return false;
        }
    }
    return deal_hand(*deck, hands, vira);
}

static bool play_interactive_hand(
    struct truco_hand *hand, struct truco_match *match,
    struct truco_card_list *hands[TRUCO_PLAYER_COUNT], bool iron_hand) {
    while (!truco_hand_is_complete(hand)) {
        const size_t player = truco_hand_current_player(hand);
        if (player >= TRUCO_PLAYER_COUNT) {
            return false;
        }
        show_current_hand(player, hands[player]);
        unsigned int signal_code;
        if (truco_hand_partner_signal(hand, player, &signal_code)) {
            printf("Partner signal code: %u (opaque; no built-in meaning).\n",
                   signal_code);
        }
        char input[64];
        printf(
            iron_hand
                ? "Action [card <index> [hidden]/signal <0..255>], stake %u: "
                : "Action [card <index> [hidden]/signal <0..255>/raise], "
                  "stake %u: ",
            truco_match_hand_stake(match));
        fflush(stdout);
        if (!read_line("", input, sizeof(input))) {
            return false;
        }
        char action[32];
        char argument[32];
        char extra[32];
        const int fields =
            sscanf(input, "%31s %31s %31s", action, argument, extra);
        if ((fields == 2 || fields == 3) && strcmp(action, "card") == 0) {
            size_t index;
            struct truco_card card;
            const bool hidden = fields == 3 && strcmp(extra, "hidden") == 0;
            const bool valid_form = fields == 2 || hidden;
            if (!parse_size(argument, &index) ||
                !truco_card_list_get(hands[player], index, &card) ||
                !valid_form ||
                (hidden ? !truco_hand_play_hidden(hand, card)
                        : !truco_hand_play(hand, card))) {
                puts("Invalid card choice; try again.");
            } else if (hidden) {
                printf("Player %zu plays a card face down.\n", player);
            }
            continue;
        }
        if (fields == 2 && strcmp(action, "signal") == 0) {
            size_t code;
            if (!parse_size(argument, &code) || code > 255 ||
                !truco_hand_signal(hand, player, (unsigned int)code)) {
                puts("Signal code must be from 0 through 255.");
            }
            continue;
        }
        if (fields == 1 && strcmp(action, "raise") == 0) {
            if (iron_hand || !truco_hand_request_raise(hand, player)) {
                puts("Raise is unavailable in this state.");
                continue;
            }
            if (truco_hand_is_complete(hand)) {
                printf("Team %d wins immediately.\n", truco_hand_winner(hand));
                break;
            }
            bool accept;
            const int responding_team = 1 - (int)(player % 2);
            printf("Team %d offered a raise to %u points.\n", (int)(player % 2),
                   truco_match_hand_stake(match) == 1   ? 3
                   : truco_match_hand_stake(match) == 3 ? 6
                   : truco_match_hand_stake(match) == 6 ? 9
                                                        : 12);
            if (!read_answer("Accept? (y/n): ", &accept) ||
                !truco_hand_respond_raise(hand, responding_team, accept)) {
                return false;
            }
            continue;
        }
        puts("Enter 'card <index> [hidden]', 'signal <0..255>', or 'raise'.");
    }
    return true;
}

int main(void) {
    uint32_t seed;
    if (!read_seed(&seed)) {
        fputs("Expected a seed from 0 through 4294967295.\n", stderr);
        return 2;
    }
    struct truco_match *match = truco_match_create();
    if (match == NULL) {
        return 1;
    }

    uint32_t hand_seed = seed;
    size_t hands_played = 0;
    bool success = true;
    while (truco_match_winner(match) == TRUCO_MATCH_ACTIVE) {
        const int score_zero = truco_match_score(match, TRUCO_TEAM_ZERO);
        const int score_one = truco_match_score(match, TRUCO_TEAM_ONE);
        const bool is_iron_hand = score_zero == 11 && score_one == 11;
        struct truco_card_list *deck = NULL;
        struct truco_card_list *cards[TRUCO_PLAYER_COUNT] = {NULL};
        struct truco_hand *hand = NULL;
        struct truco_card vira;
        if (!prepare_deal(hand_seed, &deck, cards, &vira)) {
            destroy_hand_state(deck, cards, hand);
            success = false;
            break;
        }

        if (is_iron_hand) {
            unsigned int points_awarded;
            if (!read_point_award(&points_awarded) ||
                !truco_match_start_iron_hand(match, points_awarded)) {
                destroy_hand_state(deck, cards, hand);
                success = false;
                break;
            }
        } else {
            enum truco_hand_eleven_choice choice = TRUCO_HAND_ELEVEN_NONE;
            if (score_zero == 11 || score_one == 11) {
                const int team_at_eleven = score_zero == 11 ? 0 : 1;
                show_partner_hand(team_at_eleven, cards);
                bool accept;
                if (!read_answer(
                        team_at_eleven == 0
                            ? "Team 0 at 11: accept three points? (y/n): "
                            : "Team 1 at 11: accept three points? (y/n): ",
                        &accept)) {
                    destroy_hand_state(deck, cards, hand);
                    success = false;
                    break;
                }
                choice = accept ? TRUCO_HAND_ELEVEN_ACCEPT
                                : TRUCO_HAND_ELEVEN_DECLINE;
            }
            const enum truco_hand_start_result started =
                truco_match_start_hand(match, choice);
            if (started == TRUCO_HAND_ELEVEN_DECLINED) {
                printf("Team %d declined; opponents score one point.\n",
                       score_zero == 11 ? 0 : 1);
                destroy_hand_state(deck, cards, hand);
                hand_seed += UINT32_C(0x9e3779b9);
                continue;
            }
            if (started != TRUCO_HAND_STARTED &&
                started != TRUCO_HAND_ELEVEN_ACCEPTED) {
                destroy_hand_state(deck, cards, hand);
                success = false;
                break;
            }
        }
        const enum truco_rank manilha_rank = truco_manilha_rank(vira.rank);
        const unsigned int stake = truco_match_hand_stake(match);
        hand = truco_hand_create(cards, match, manilha_rank, 0);
        if (hand == NULL) {
            destroy_hand_state(deck, cards, hand);
            success = false;
            break;
        }
        ++hands_played;
        printf("Hand %zu (%s), vira %s/%s, manilha %s, stake %u.\n",
               hands_played, is_iron_hand ? "iron" : "normal",
               truco_rank_name(vira.rank), truco_suit_name(vira.suit),
               truco_rank_name(manilha_rank), stake);
        if (!play_interactive_hand(hand, match, cards, is_iron_hand)) {
            destroy_hand_state(deck, cards, hand);
            success = false;
            break;
        }
        printf("Hand winner %d; score %d-%d.\n", truco_hand_winner(hand),
               truco_match_score(match, TRUCO_TEAM_ZERO),
               truco_match_score(match, TRUCO_TEAM_ONE));
        destroy_hand_state(deck, cards, hand);
        hand_seed += UINT32_C(0x9e3779b9);
    }

    if (success) {
        printf("Match winner %d; final score %d-%d; hands %zu.\n",
               truco_match_winner(match),
               truco_match_score(match, TRUCO_TEAM_ZERO),
               truco_match_score(match, TRUCO_TEAM_ONE), hands_played);
    }
    truco_match_destroy(match);
    return success ? 0 : 1;
}
