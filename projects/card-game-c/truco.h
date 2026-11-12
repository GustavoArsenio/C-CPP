#ifndef C_CPP_TRUCO_H
#define C_CPP_TRUCO_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

enum truco_rank {
    TRUCO_FOUR,
    TRUCO_FIVE,
    TRUCO_SIX,
    TRUCO_SEVEN,
    TRUCO_QUEEN,
    TRUCO_JACK,
    TRUCO_KING,
    TRUCO_ACE,
    TRUCO_TWO,
    TRUCO_THREE
};

enum truco_suit { TRUCO_DIAMONDS, TRUCO_SPADES, TRUCO_HEARTS, TRUCO_CLUBS };

struct truco_card {
    enum truco_rank rank;
    enum truco_suit suit;
};

struct truco_card_list;
struct truco_round;
struct truco_match;
struct truco_hand;

struct truco_card_list *truco_card_list_create(void);
struct truco_card_list *truco_standard_deck_create(void);
void truco_card_list_destroy(struct truco_card_list *list);
size_t truco_card_list_size(const struct truco_card_list *list);
bool truco_card_list_append(struct truco_card_list *list,
                            struct truco_card card);
bool truco_card_list_get(const struct truco_card_list *list, size_t index,
                         struct truco_card *card);
bool truco_card_list_pop_front(struct truco_card_list *list,
                               struct truco_card *card);
bool truco_card_list_move_card(struct truco_card_list *source,
                               struct truco_card_list *destination,
                               struct truco_card card);
bool truco_deck_shuffle(struct truco_card_list *deck, uint32_t seed);
enum truco_rank truco_manilha_rank(enum truco_rank vira_rank);
int truco_card_compare(struct truco_card left, struct truco_card right,
                       enum truco_rank manilha_rank);
const char *truco_rank_name(enum truco_rank rank);
const char *truco_suit_name(enum truco_suit suit);

enum { TRUCO_PLAYER_COUNT = 4 };
enum { TRUCO_HAND_INVALID = -2, TRUCO_HAND_DRAW = -1 };

/* The round borrows distinct, non-empty hands and owns its played-card pile. */
struct truco_round *truco_round_create(
    struct truco_card_list *hands[TRUCO_PLAYER_COUNT],
    enum truco_rank manilha_rank, size_t starting_player);
struct truco_round *truco_round_create_for_trick(
    struct truco_card_list *hands[TRUCO_PLAYER_COUNT],
    enum truco_rank manilha_rank, size_t starting_player, size_t trick_number);
void truco_round_destroy(struct truco_round *round);
/* Returns TRUCO_PLAYER_COUNT when the round is complete or round is NULL. */
size_t truco_round_current_player(const struct truco_round *round);
bool truco_round_play(struct truco_round *round, struct truco_card card);
/* A face-down play is allowed in tricks 2 and 3 of a hand, never trick 1.
 * The legacy create function treats a standalone round as trick 1.
 * The hidden card is discarded from trick comparison. */
bool truco_round_play_hidden(struct truco_round *round, struct truco_card card);
bool truco_round_play_was_hidden(const struct truco_round *round,
                                 size_t play_index);
bool truco_round_is_complete(const struct truco_round *round);
/* On success, player is a seat index or TRUCO_HAND_DRAW for a tied trick. */
bool truco_round_winner(const struct truco_round *round, int *player);
const struct truco_card_list *truco_round_played_cards(
    const struct truco_round *round);
/* Returns partnership 0/1, TRUCO_HAND_DRAW, or TRUCO_HAND_INVALID. */
int truco_resolve_hand(const int round_winners[3]);

/* A hand borrows its three-card hands and active match. The caller chooses a
 * fixed starting player for each trick because the assignment does not define
 * how the next trick's leader is selected. */
struct truco_hand *truco_hand_create(
    struct truco_card_list *hands[TRUCO_PLAYER_COUNT],
    struct truco_match *match, enum truco_rank manilha_rank,
    size_t starting_player);
void truco_hand_destroy(struct truco_hand *hand);
size_t truco_hand_current_player(const struct truco_hand *hand);
/* 1-based current trick, or number already played after completion. */
size_t truco_hand_trick_number(const struct truco_hand *hand);
bool truco_hand_has_pending_raise(const struct truco_hand *hand);
bool truco_hand_is_complete(const struct truco_hand *hand);
bool truco_hand_play(struct truco_hand *hand, struct truco_card card);
bool truco_hand_play_hidden(struct truco_hand *hand, struct truco_card card);
/* trick_number is 1-based; play_index is zero-based in turn order. */
bool truco_hand_trick_play_was_hidden(const struct truco_hand *hand,
                                      size_t trick_number, size_t play_index);
/* Signal codes are opaque values: the assignment allows signaling but does
 * not define conventions. A team's latest code can be read by its partner. */
bool truco_hand_signal(struct truco_hand *hand, size_t player,
                       unsigned int code);
bool truco_hand_partner_signal(const struct truco_hand *hand, size_t player,
                               unsigned int *code);
bool truco_hand_request_raise(struct truco_hand *hand, size_t player);
bool truco_hand_respond_raise(struct truco_hand *hand, int responding_team,
                              bool accept);
/* Returns winning team, TRUCO_HAND_DRAW, or TRUCO_HAND_INVALID until complete.
 */
int truco_hand_winner(const struct truco_hand *hand);

enum { TRUCO_MATCH_ACTIVE = -1, TRUCO_MATCH_INVALID = -2 };
enum { TRUCO_TEAM_NONE = -1, TRUCO_TEAM_ZERO = 0, TRUCO_TEAM_ONE = 1 };

enum truco_hand_start_result {
    TRUCO_HAND_START_ERROR = -1,
    TRUCO_HAND_STARTED = 0,
    TRUCO_HAND_ELEVEN_ACCEPTED = 1,
    TRUCO_HAND_ELEVEN_DECLINED = 2,
    TRUCO_HAND_IRON_POLICY_REQUIRED = 3
};

enum truco_hand_eleven_choice {
    TRUCO_HAND_ELEVEN_NONE,
    TRUCO_HAND_ELEVEN_ACCEPT,
    TRUCO_HAND_ELEVEN_DECLINE
};

struct truco_match *truco_match_create(void);
void truco_match_destroy(struct truco_match *match);
int truco_match_score(const struct truco_match *match, int team);
int truco_match_winner(const struct truco_match *match);
unsigned int truco_match_hand_stake(const struct truco_match *match);
bool truco_match_hand_is_active(const struct truco_match *match);
bool truco_match_has_pending_raise(const struct truco_match *match);
enum truco_hand_start_result truco_match_start_hand(
    struct truco_match *match, enum truco_hand_eleven_choice choice);
/* Starts an iron hand only when both teams are at 11. The caller supplies
 * its point award because the assignment does not define the stake. */
bool truco_match_start_iron_hand(struct truco_match *match,
                                 unsigned int points_awarded);
bool truco_match_request_raise(struct truco_match *match, size_t player,
                               size_t current_player);
bool truco_match_respond_raise(struct truco_match *match, int responding_team,
                               bool accept);
bool truco_match_finish_hand(struct truco_match *match, int winning_team);
/* The PDF does not define iron-hand scoring; the caller supplies the outcome.
 */
bool truco_match_resolve_iron_hand(struct truco_match *match, int winning_team,
                                   unsigned int points_awarded);

#endif
