#include "truco.h"

#include <stdlib.h>

struct card_node {
    struct truco_card card;
    struct card_node *next;
};

struct truco_card_list {
    struct card_node *head;
    struct card_node *tail;
    size_t size;
};

struct player_order_node {
    size_t player;
    struct player_order_node *next;
};

struct truco_round {
    struct truco_card_list *hands[TRUCO_PLAYER_COUNT];
    struct truco_card_list *played;
    struct player_order_node order[TRUCO_PLAYER_COUNT];
    struct player_order_node *current;
    enum truco_rank manilha_rank;
    size_t play_count;
    bool hidden_plays[TRUCO_PLAYER_COUNT];
    bool hidden_allowed;
    size_t winning_player;
    struct truco_card winning_card;
    bool has_winning_card;
    bool tied_for_high_card;
};

struct truco_hand {
    struct truco_card_list *hands[TRUCO_PLAYER_COUNT];
    struct truco_match *match;
    struct truco_round *rounds[3];
    struct truco_round *round;
    enum truco_rank manilha_rank;
    size_t starting_player;
    size_t trick_number;
    int trick_winners[3];
    int winner;
    int raiser_team;
    unsigned int signal_code[2];
    size_t signal_sender[2];
    bool signal_available[2];
    bool complete;
};

struct truco_match {
    unsigned int score[2];
    unsigned int hand_stake;
    unsigned int requested_stake;
    int pending_team;
    int winner;
    bool hand_active;
    bool iron_hand;
    bool raise_pending;
};

static void append_node(struct truco_card_list *list, struct card_node *node) {
    node->next = NULL;
    if (list->tail == NULL) {
        list->head = node;
    } else {
        list->tail->next = node;
    }
    list->tail = node;
    ++list->size;
}

struct truco_card_list *truco_card_list_create(void) {
    return calloc(1, sizeof(struct truco_card_list));
}

struct truco_card_list *truco_standard_deck_create(void) {
    struct truco_card_list *deck = truco_card_list_create();
    if (deck == NULL) {
        return NULL;
    }
    for (int suit = TRUCO_DIAMONDS; suit <= TRUCO_CLUBS; ++suit) {
        for (int rank = TRUCO_FOUR; rank <= TRUCO_THREE; ++rank) {
            struct truco_card card = {(enum truco_rank)rank,
                                      (enum truco_suit)suit};
            if (!truco_card_list_append(deck, card)) {
                truco_card_list_destroy(deck);
                return NULL;
            }
        }
    }
    return deck;
}

void truco_card_list_destroy(struct truco_card_list *list) {
    if (list == NULL) {
        return;
    }
    struct card_node *node = list->head;
    while (node != NULL) {
        struct card_node *next = node->next;
        free(node);
        node = next;
    }
    free(list);
}

size_t truco_card_list_size(const struct truco_card_list *list) {
    return list == NULL ? 0 : list->size;
}

bool truco_card_list_append(struct truco_card_list *list,
                            struct truco_card card) {
    if (list == NULL || card.rank < TRUCO_FOUR || card.rank > TRUCO_THREE ||
        card.suit < TRUCO_DIAMONDS || card.suit > TRUCO_CLUBS) {
        return false;
    }
    struct card_node *node = malloc(sizeof(*node));
    if (node == NULL) {
        return false;
    }
    node->card = card;
    append_node(list, node);
    return true;
}

bool truco_card_list_get(const struct truco_card_list *list, size_t index,
                         struct truco_card *card) {
    if (list == NULL || card == NULL || index >= list->size) {
        return false;
    }
    const struct card_node *node = list->head;
    for (size_t current = 0; current < index; ++current) {
        node = node->next;
    }
    *card = node->card;
    return true;
}

bool truco_card_list_pop_front(struct truco_card_list *list,
                               struct truco_card *card) {
    if (list == NULL || card == NULL || list->head == NULL) {
        return false;
    }
    struct card_node *node = list->head;
    *card = node->card;
    list->head = node->next;
    if (list->head == NULL) {
        list->tail = NULL;
    }
    --list->size;
    free(node);
    return true;
}

bool truco_card_list_move_card(struct truco_card_list *source,
                               struct truco_card_list *destination,
                               struct truco_card card) {
    if (source == NULL || destination == NULL || source == destination) {
        return false;
    }
    struct card_node **link = &source->head;
    while (*link != NULL && ((*link)->card.rank != card.rank ||
                             (*link)->card.suit != card.suit)) {
        link = &(*link)->next;
    }
    if (*link == NULL) {
        return false;
    }
    struct card_node *node = *link;
    *link = node->next;
    if (source->tail == node) {
        source->tail = NULL;
        if (source->head != NULL) {
            source->tail = source->head;
            while (source->tail->next != NULL) {
                source->tail = source->tail->next;
            }
        }
    }
    --source->size;
    append_node(destination, node);
    return true;
}

static uint32_t next_random(uint32_t *state) {
    uint32_t value = *state;
    value ^= value << 13;
    value ^= value >> 17;
    value ^= value << 5;
    *state = value;
    return value;
}

static size_t bounded_random(uint32_t *state, size_t bound) {
    const uint32_t range = (uint32_t)bound;
    const uint32_t threshold = (uint32_t)(-range) % range;
    uint32_t value;
    do {
        value = next_random(state);
    } while (value < threshold);
    return (size_t)(value % range);
}

bool truco_deck_shuffle(struct truco_card_list *deck, uint32_t seed) {
    if (deck == NULL || deck->size > UINT32_MAX) {
        return false;
    }
    if (deck->size < 2) {
        return true;
    }
    uint32_t random_state = seed == 0 ? UINT32_C(0x9e3779b9) : seed;
    struct truco_card_list shuffled = {0};
    while (deck->head != NULL) {
        size_t selected = bounded_random(&random_state, deck->size);
        struct card_node **link = &deck->head;
        for (size_t index = 0; index < selected; ++index) {
            link = &(*link)->next;
        }
        struct card_node *node = *link;
        *link = node->next;
        if (deck->tail == node) {
            deck->tail = NULL;
            if (deck->head != NULL) {
                deck->tail = deck->head;
                while (deck->tail->next != NULL) {
                    deck->tail = deck->tail->next;
                }
            }
        }
        --deck->size;
        node->next = NULL;
        if (shuffled.tail == NULL) {
            shuffled.head = node;
        } else {
            shuffled.tail->next = node;
        }
        shuffled.tail = node;
        ++shuffled.size;
    }
    deck->head = shuffled.head;
    deck->tail = shuffled.tail;
    deck->size = shuffled.size;
    return true;
}

enum truco_rank truco_manilha_rank(enum truco_rank vira_rank) {
    return (vira_rank < TRUCO_FOUR || vira_rank >= TRUCO_THREE)
               ? TRUCO_FOUR
               : (enum truco_rank)(vira_rank + 1);
}

static int suit_strength(enum truco_suit suit) {
    switch (suit) {
        case TRUCO_CLUBS:
            return 4;
        case TRUCO_HEARTS:
            return 3;
        case TRUCO_SPADES:
            return 2;
        case TRUCO_DIAMONDS:
            return 1;
    }
    return 0;
}

int truco_card_compare(struct truco_card left, struct truco_card right,
                       enum truco_rank manilha_rank) {
    const bool left_manilha = left.rank == manilha_rank;
    const bool right_manilha = right.rank == manilha_rank;
    if (left_manilha != right_manilha) {
        return left_manilha ? 1 : -1;
    }
    if (left_manilha) {
        const int left_suit = suit_strength(left.suit);
        const int right_suit = suit_strength(right.suit);
        return (left_suit > right_suit) - (left_suit < right_suit);
    }
    return (left.rank > right.rank) - (left.rank < right.rank);
}

const char *truco_rank_name(enum truco_rank rank) {
    static const char *const names[] = {"4", "5", "6", "7", "Q",
                                        "J", "K", "A", "2", "3"};
    return rank >= TRUCO_FOUR && rank <= TRUCO_THREE ? names[rank] : "?";
}

const char *truco_suit_name(enum truco_suit suit) {
    static const char *const names[] = {"diamonds", "spades", "hearts",
                                        "clubs"};
    return suit >= TRUCO_DIAMONDS && suit <= TRUCO_CLUBS ? names[suit] : "?";
}

static struct truco_round *round_create(
    struct truco_card_list *hands[TRUCO_PLAYER_COUNT],
    enum truco_rank manilha_rank, size_t starting_player, bool hidden_allowed) {
    if (hands == NULL || starting_player >= TRUCO_PLAYER_COUNT ||
        manilha_rank < TRUCO_FOUR || manilha_rank > TRUCO_THREE) {
        return NULL;
    }
    for (size_t player = 0; player < TRUCO_PLAYER_COUNT; ++player) {
        if (hands[player] == NULL || truco_card_list_size(hands[player]) == 0) {
            return NULL;
        }
        for (size_t earlier = 0; earlier < player; ++earlier) {
            if (hands[player] == hands[earlier]) {
                return NULL;
            }
        }
    }
    struct truco_round *round = calloc(1, sizeof(*round));
    if (round == NULL) {
        return NULL;
    }
    round->played = truco_card_list_create();
    if (round->played == NULL) {
        free(round);
        return NULL;
    }
    round->manilha_rank = manilha_rank;
    round->hidden_allowed = hidden_allowed;
    for (size_t player = 0; player < TRUCO_PLAYER_COUNT; ++player) {
        round->hands[player] = hands[player];
        round->order[player].player = player;
        round->order[player].next =
            &round->order[(player + 1) % TRUCO_PLAYER_COUNT];
    }
    round->current = &round->order[starting_player];
    return round;
}

struct truco_round *truco_round_create(
    struct truco_card_list *hands[TRUCO_PLAYER_COUNT],
    enum truco_rank manilha_rank, size_t starting_player) {
    return round_create(hands, manilha_rank, starting_player, false);
}

struct truco_round *truco_round_create_for_trick(
    struct truco_card_list *hands[TRUCO_PLAYER_COUNT],
    enum truco_rank manilha_rank, size_t starting_player, size_t trick_number) {
    if (trick_number < 1 || trick_number > 3) {
        return NULL;
    }
    return round_create(hands, manilha_rank, starting_player, trick_number > 1);
}

void truco_round_destroy(struct truco_round *round) {
    if (round == NULL) {
        return;
    }
    truco_card_list_destroy(round->played);
    free(round);
}

size_t truco_round_current_player(const struct truco_round *round) {
    return round == NULL || round->play_count == TRUCO_PLAYER_COUNT
               ? TRUCO_PLAYER_COUNT
               : round->current->player;
}

static bool round_play(struct truco_round *round, struct truco_card card,
                       bool hidden) {
    if (round == NULL || round->play_count == TRUCO_PLAYER_COUNT) {
        return false;
    }
    if (hidden && !round->hidden_allowed) {
        return false;
    }
    const size_t player = round->current->player;
    if (!truco_card_list_move_card(round->hands[player], round->played, card)) {
        return false;
    }
    round->hidden_plays[round->play_count] = hidden;
    if (hidden) {
        /* A face-down card counts as a play but cannot win or tie a trick. */
    } else if (!round->has_winning_card) {
        round->winning_card = card;
        round->winning_player = player;
        round->has_winning_card = true;
    } else {
        const int comparison =
            truco_card_compare(card, round->winning_card, round->manilha_rank);
        if (comparison > 0) {
            round->winning_card = card;
            round->winning_player = player;
            round->tied_for_high_card = false;
        } else if (comparison == 0) {
            round->tied_for_high_card = true;
        }
    }
    ++round->play_count;
    round->current = round->current->next;
    return true;
}

bool truco_round_play(struct truco_round *round, struct truco_card card) {
    return round_play(round, card, false);
}

bool truco_round_play_hidden(struct truco_round *round,
                             struct truco_card card) {
    return round_play(round, card, true);
}

bool truco_round_play_was_hidden(const struct truco_round *round,
                                 size_t play_index) {
    return round != NULL && play_index < round->play_count &&
           round->hidden_plays[play_index];
}

bool truco_round_is_complete(const struct truco_round *round) {
    return round != NULL && round->play_count == TRUCO_PLAYER_COUNT;
}

bool truco_round_winner(const struct truco_round *round, int *player) {
    if (!truco_round_is_complete(round) || player == NULL) {
        return false;
    }
    *player = !round->has_winning_card || round->tied_for_high_card
                  ? TRUCO_HAND_DRAW
                  : (int)round->winning_player;
    return true;
}

const struct truco_card_list *truco_round_played_cards(
    const struct truco_round *round) {
    return round == NULL ? NULL : round->played;
}

int truco_resolve_hand(const int round_winners[3]) {
    if (round_winners == NULL) {
        return TRUCO_HAND_INVALID;
    }
    for (size_t round = 0; round < 3; ++round) {
        if (round_winners[round] < TRUCO_HAND_DRAW ||
            round_winners[round] >= TRUCO_PLAYER_COUNT) {
            return TRUCO_HAND_INVALID;
        }
    }
    if (round_winners[0] == TRUCO_HAND_DRAW &&
        round_winners[1] == TRUCO_HAND_DRAW &&
        round_winners[2] == TRUCO_HAND_DRAW) {
        return TRUCO_HAND_DRAW;
    }
    if (round_winners[0] == TRUCO_HAND_DRAW) {
        return round_winners[1] == TRUCO_HAND_DRAW ? round_winners[2] % 2
                                                   : round_winners[1] % 2;
    }
    if (round_winners[1] == TRUCO_HAND_DRAW ||
        round_winners[2] == TRUCO_HAND_DRAW) {
        return round_winners[0] % 2;
    }
    if (round_winners[0] % 2 == round_winners[1] % 2) {
        return round_winners[0] % 2;
    }
    return round_winners[2] % 2;
}

static bool valid_team(int team) {
    return team == TRUCO_TEAM_ZERO || team == TRUCO_TEAM_ONE;
}

static bool award_points(struct truco_match *match, int team,
                         unsigned int points) {
    if (!valid_team(team) || points == 0) {
        return false;
    }
    const unsigned int remaining = 12U - match->score[team];
    match->score[team] += points > remaining ? remaining : points;
    if (match->score[team] >= 12U) {
        match->winner = team;
    }
    return true;
}

struct truco_match *truco_match_create(void) {
    struct truco_match *match = calloc(1, sizeof(*match));
    if (match != NULL) {
        match->winner = TRUCO_MATCH_ACTIVE;
        match->pending_team = TRUCO_TEAM_NONE;
    }
    return match;
}

void truco_match_destroy(struct truco_match *match) {
    free(match);
}

int truco_match_score(const struct truco_match *match, int team) {
    if (match == NULL || !valid_team(team)) {
        return TRUCO_MATCH_INVALID;
    }
    return (int)match->score[team];
}

int truco_match_winner(const struct truco_match *match) {
    return match == NULL ? TRUCO_MATCH_INVALID : match->winner;
}

unsigned int truco_match_hand_stake(const struct truco_match *match) {
    return match == NULL || !match->hand_active ? 0 : match->hand_stake;
}

bool truco_match_hand_is_active(const struct truco_match *match) {
    return match != NULL && match->hand_active;
}

bool truco_match_has_pending_raise(const struct truco_match *match) {
    return match != NULL && match->raise_pending;
}

enum truco_hand_start_result truco_match_start_hand(
    struct truco_match *match, enum truco_hand_eleven_choice choice) {
    if (match == NULL || match->winner != TRUCO_MATCH_ACTIVE ||
        match->hand_active || match->raise_pending) {
        return TRUCO_HAND_START_ERROR;
    }
    const bool team_zero_at_eleven = match->score[TRUCO_TEAM_ZERO] == 11U;
    const bool team_one_at_eleven = match->score[TRUCO_TEAM_ONE] == 11U;
    if (team_zero_at_eleven && team_one_at_eleven) {
        return TRUCO_HAND_IRON_POLICY_REQUIRED;
    }
    if (!team_zero_at_eleven && !team_one_at_eleven) {
        if (choice != TRUCO_HAND_ELEVEN_NONE) {
            return TRUCO_HAND_START_ERROR;
        }
        match->hand_stake = 1;
        match->hand_active = true;
        match->iron_hand = false;
        return TRUCO_HAND_STARTED;
    }
    if (choice == TRUCO_HAND_ELEVEN_ACCEPT) {
        match->hand_stake = 3;
        match->hand_active = true;
        match->iron_hand = false;
        return TRUCO_HAND_ELEVEN_ACCEPTED;
    }
    if (choice == TRUCO_HAND_ELEVEN_DECLINE) {
        const int opponents =
            team_zero_at_eleven ? TRUCO_TEAM_ONE : TRUCO_TEAM_ZERO;
        if (!award_points(match, opponents, 1)) {
            return TRUCO_HAND_START_ERROR;
        }
        return TRUCO_HAND_ELEVEN_DECLINED;
    }
    return TRUCO_HAND_START_ERROR;
}

bool truco_match_start_iron_hand(struct truco_match *match,
                                 unsigned int points_awarded) {
    if (match == NULL || match->winner != TRUCO_MATCH_ACTIVE ||
        match->hand_active || match->raise_pending || points_awarded == 0 ||
        points_awarded > 12U || match->score[TRUCO_TEAM_ZERO] != 11U ||
        match->score[TRUCO_TEAM_ONE] != 11U) {
        return false;
    }
    match->hand_stake = points_awarded;
    match->hand_active = true;
    match->iron_hand = true;
    return true;
}

bool truco_match_request_raise(struct truco_match *match, size_t player,
                               size_t current_player) {
    if (match == NULL || match->winner != TRUCO_MATCH_ACTIVE ||
        !match->hand_active || match->iron_hand || match->raise_pending ||
        player >= TRUCO_PLAYER_COUNT || player != current_player) {
        return false;
    }
    const int team = (int)(player % 2);
    if (match->score[team] == 11U) {
        match->winner = 1 - team;
        match->hand_active = false;
        match->hand_stake = 0;
        return true;
    }
    switch (match->hand_stake) {
        case 1:
            match->requested_stake = 3;
            break;
        case 3:
            match->requested_stake = 6;
            break;
        case 6:
            match->requested_stake = 9;
            break;
        case 9:
            match->requested_stake = 12;
            break;
        default:
            return false;
    }
    match->pending_team = team;
    match->raise_pending = true;
    return true;
}

bool truco_match_respond_raise(struct truco_match *match, int responding_team,
                               bool accept) {
    if (match == NULL || match->winner != TRUCO_MATCH_ACTIVE ||
        !match->hand_active || !match->raise_pending ||
        !valid_team(responding_team) ||
        responding_team == match->pending_team) {
        return false;
    }
    const int challenger = match->pending_team;
    match->raise_pending = false;
    match->pending_team = TRUCO_TEAM_NONE;
    if (accept) {
        match->hand_stake = match->requested_stake;
        match->requested_stake = 0;
        return true;
    }
    match->requested_stake = 0;
    const unsigned int refused_stake = match->hand_stake;
    match->hand_active = false;
    match->hand_stake = 0;
    return award_points(match, challenger, refused_stake);
}

bool truco_match_finish_hand(struct truco_match *match, int winning_team) {
    if (match == NULL || match->winner != TRUCO_MATCH_ACTIVE ||
        !match->hand_active || match->raise_pending ||
        (winning_team != TRUCO_HAND_DRAW && !valid_team(winning_team))) {
        return false;
    }
    const unsigned int stake = match->hand_stake;
    match->hand_stake = 0;
    match->hand_active = false;
    match->iron_hand = false;
    return winning_team == TRUCO_HAND_DRAW ||
           award_points(match, winning_team, stake);
}

struct truco_hand *truco_hand_create(
    struct truco_card_list *hands[TRUCO_PLAYER_COUNT],
    struct truco_match *match, enum truco_rank manilha_rank,
    size_t starting_player) {
    if (hands == NULL || match == NULL || !match->hand_active ||
        match->raise_pending || starting_player >= TRUCO_PLAYER_COUNT ||
        manilha_rank < TRUCO_FOUR || manilha_rank > TRUCO_THREE) {
        return NULL;
    }
    for (size_t player = 0; player < TRUCO_PLAYER_COUNT; ++player) {
        if (hands[player] == NULL || truco_card_list_size(hands[player]) != 3) {
            return NULL;
        }
    }
    struct truco_hand *hand = calloc(1, sizeof(*hand));
    if (hand == NULL) {
        return NULL;
    }
    for (size_t player = 0; player < TRUCO_PLAYER_COUNT; ++player) {
        hand->hands[player] = hands[player];
    }
    hand->match = match;
    hand->manilha_rank = manilha_rank;
    hand->starting_player = starting_player;
    hand->winner = TRUCO_HAND_INVALID;
    hand->raiser_team = TRUCO_TEAM_NONE;
    for (size_t trick = 0; trick < 3; ++trick) {
        hand->rounds[trick] =
            round_create(hands, manilha_rank, starting_player, trick > 0);
        if (hand->rounds[trick] == NULL) {
            truco_hand_destroy(hand);
            return NULL;
        }
    }
    hand->round = hand->rounds[0];
    return hand;
}

void truco_hand_destroy(struct truco_hand *hand) {
    if (hand == NULL) {
        return;
    }
    for (size_t trick = 0; trick < 3; ++trick) {
        truco_round_destroy(hand->rounds[trick]);
    }
    free(hand);
}

size_t truco_hand_current_player(const struct truco_hand *hand) {
    return hand == NULL || hand->complete || hand->round == NULL
               ? TRUCO_PLAYER_COUNT
               : truco_round_current_player(hand->round);
}

bool truco_hand_signal(struct truco_hand *hand, size_t player,
                       unsigned int code) {
    if (hand == NULL || hand->complete || player >= TRUCO_PLAYER_COUNT ||
        code > 255 || truco_hand_current_player(hand) != player ||
        truco_match_has_pending_raise(hand->match)) {
        return false;
    }
    const size_t team = player % 2;
    hand->signal_code[team] = code;
    hand->signal_sender[team] = player;
    hand->signal_available[team] = true;
    return true;
}

bool truco_hand_partner_signal(const struct truco_hand *hand, size_t player,
                               unsigned int *code) {
    if (hand == NULL || code == NULL || player >= TRUCO_PLAYER_COUNT ||
        truco_hand_current_player(hand) != player) {
        return false;
    }
    const size_t team = player % 2;
    if (!hand->signal_available[team] || hand->signal_sender[team] == player) {
        return false;
    }
    *code = hand->signal_code[team];
    return true;
}

size_t truco_hand_trick_number(const struct truco_hand *hand) {
    if (hand == NULL) {
        return 0;
    }
    return hand->complete ? hand->trick_number : hand->trick_number + 1;
}

bool truco_hand_has_pending_raise(const struct truco_hand *hand) {
    return hand != NULL && truco_match_has_pending_raise(hand->match);
}

bool truco_hand_is_complete(const struct truco_hand *hand) {
    return hand != NULL && hand->complete;
}

static bool finish_truco_hand_with_winner(struct truco_hand *hand,
                                          int winning_team) {
    if ((winning_team != TRUCO_HAND_DRAW && !valid_team(winning_team)) ||
        !truco_match_finish_hand(hand->match, winning_team)) {
        hand->winner = TRUCO_HAND_INVALID;
        return false;
    }
    hand->winner = winning_team;
    hand->complete = true;
    return true;
}

static bool resolve_hand_prefix(const int winners[3], size_t rounds_played,
                                int *winning_team) {
    if (winners == NULL || winning_team == NULL || rounds_played < 2 ||
        rounds_played > 3) {
        return false;
    }
    if (rounds_played == 2) {
        const int first = winners[0];
        const int second = winners[1];
        if (first == TRUCO_HAND_DRAW && second == TRUCO_HAND_DRAW) {
            return false;
        }
        if (first == TRUCO_HAND_DRAW) {
            *winning_team = second % 2;
            return true;
        }
        if (second == TRUCO_HAND_DRAW || first % 2 == second % 2) {
            *winning_team = first % 2;
            return true;
        }
        return false;
    }
    *winning_team = truco_resolve_hand(winners);
    return *winning_team != TRUCO_HAND_INVALID;
}

static bool finish_truco_hand(struct truco_hand *hand) {
    return finish_truco_hand_with_winner(
        hand, truco_resolve_hand(hand->trick_winners));
}

static bool complete_hand_play(struct truco_hand *hand);

bool truco_hand_play(struct truco_hand *hand, struct truco_card card) {
    if (hand == NULL || hand->complete || hand->round == NULL ||
        truco_match_has_pending_raise(hand->match) ||
        !truco_match_hand_is_active(hand->match) ||
        !truco_round_play(hand->round, card)) {
        return false;
    }
    return complete_hand_play(hand);
}

bool truco_hand_play_hidden(struct truco_hand *hand, struct truco_card card) {
    if (hand == NULL || hand->complete || hand->round == NULL ||
        truco_match_has_pending_raise(hand->match) ||
        !truco_match_hand_is_active(hand->match) ||
        !truco_round_play_hidden(hand->round, card)) {
        return false;
    }
    return complete_hand_play(hand);
}

bool truco_hand_trick_play_was_hidden(const struct truco_hand *hand,
                                      size_t trick_number, size_t play_index) {
    return hand != NULL && trick_number >= 1 && trick_number <= 3 &&
           truco_round_play_was_hidden(hand->rounds[trick_number - 1],
                                       play_index);
}

static bool complete_hand_play(struct truco_hand *hand) {
    if (!truco_round_is_complete(hand->round)) {
        return true;
    }
    int winner;
    if (!truco_round_winner(hand->round, &winner)) {
        return false;
    }
    hand->trick_winners[hand->trick_number++] = winner;
    hand->round = NULL;
    int winning_team;
    if (resolve_hand_prefix(hand->trick_winners, hand->trick_number,
                            &winning_team)) {
        return finish_truco_hand_with_winner(hand, winning_team);
    }
    if (hand->trick_number == 3) {
        return finish_truco_hand(hand);
    }
    hand->round = hand->rounds[hand->trick_number];
    return true;
}

bool truco_hand_request_raise(struct truco_hand *hand, size_t player) {
    if (hand == NULL || hand->complete || hand->round == NULL ||
        truco_match_has_pending_raise(hand->match)) {
        return false;
    }
    const size_t current = truco_round_current_player(hand->round);
    if (current >= TRUCO_PLAYER_COUNT || player != current ||
        !truco_match_request_raise(hand->match, player, current)) {
        return false;
    }
    hand->raiser_team = (int)(player % 2);
    if (!truco_match_hand_is_active(hand->match)) {
        hand->winner = 1 - hand->raiser_team;
        hand->complete = true;
        hand->round = NULL;
    }
    return true;
}

bool truco_hand_respond_raise(struct truco_hand *hand, int responding_team,
                              bool accept) {
    if (hand == NULL || hand->complete ||
        !truco_match_has_pending_raise(hand->match) ||
        !truco_match_respond_raise(hand->match, responding_team, accept)) {
        return false;
    }
    if (!accept) {
        hand->winner = hand->raiser_team;
        hand->complete = true;
        hand->round = NULL;
    }
    hand->raiser_team = TRUCO_TEAM_NONE;
    return true;
}

int truco_hand_winner(const struct truco_hand *hand) {
    return hand == NULL || !hand->complete ? TRUCO_HAND_INVALID : hand->winner;
}

bool truco_match_resolve_iron_hand(struct truco_match *match, int winning_team,
                                   unsigned int points_awarded) {
    if (match == NULL || match->winner != TRUCO_MATCH_ACTIVE ||
        match->hand_active || match->raise_pending ||
        match->score[TRUCO_TEAM_ZERO] != 11U ||
        match->score[TRUCO_TEAM_ONE] != 11U || points_awarded == 0 ||
        points_awarded > 12U || !valid_team(winning_team)) {
        return false;
    }
    return award_points(match, winning_team, points_awarded);
}
