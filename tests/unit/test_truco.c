#include <assert.h>

#include "truco.h"

static void assert_same_deck(const struct truco_card_list *left,
                             const struct truco_card_list *right) {
    assert(truco_card_list_size(left) == truco_card_list_size(right));
    for (size_t i = 0; i < truco_card_list_size(left); ++i) {
        struct truco_card left_card;
        struct truco_card right_card;
        assert(truco_card_list_get(left, i, &left_card));
        assert(truco_card_list_get(right, i, &right_card));
        assert(left_card.rank == right_card.rank);
        assert(left_card.suit == right_card.suit);
    }
}

static void score_team_to(struct truco_match *match, int team,
                          unsigned int target) {
    while ((unsigned int)truco_match_score(match, team) < target) {
        assert(truco_match_start_hand(match, TRUCO_HAND_ELEVEN_NONE) ==
               TRUCO_HAND_STARTED);
        assert(truco_match_finish_hand(match, team));
    }
}

static unsigned int next_stake(unsigned int stake) {
    switch (stake) {
        case 1:
            return 3;
        case 3:
            return 6;
        case 6:
            return 9;
        case 9:
            return 12;
    }
    return 0;
}

static void test_raise_refusal_at(unsigned int refused_stake) {
    struct truco_match *match = truco_match_create();
    assert(match != NULL);
    assert(truco_match_start_hand(match, TRUCO_HAND_ELEVEN_NONE) ==
           TRUCO_HAND_STARTED);
    unsigned int stake = 1;
    int challenger = TRUCO_TEAM_ZERO;
    while (stake < refused_stake) {
        assert(truco_match_request_raise(match, (size_t)challenger,
                                         (size_t)challenger));
        assert(truco_match_respond_raise(match, 1 - challenger, true));
        stake = next_stake(stake);
        challenger = 1 - challenger;
    }
    assert(truco_match_hand_stake(match) == refused_stake);
    assert(truco_match_request_raise(match, (size_t)challenger,
                                     (size_t)challenger));
    assert(truco_match_respond_raise(match, 1 - challenger, false));
    assert(truco_match_score(match, challenger) == (int)refused_stake);
    assert(!truco_match_hand_is_active(match));
    assert(!truco_match_has_pending_raise(match));
    truco_match_destroy(match);
}

static void test_match_wagers(void) {
    struct truco_match *draw_match = truco_match_create();
    assert(draw_match != NULL);
    assert(truco_match_start_hand(draw_match, TRUCO_HAND_ELEVEN_NONE) ==
           TRUCO_HAND_STARTED);
    assert(!truco_match_finish_hand(draw_match, 2));
    assert(truco_match_finish_hand(draw_match, TRUCO_HAND_DRAW));
    assert(truco_match_score(draw_match, TRUCO_TEAM_ZERO) == 0);
    assert(truco_match_score(draw_match, TRUCO_TEAM_ONE) == 0);
    assert(truco_match_winner(draw_match) == TRUCO_MATCH_ACTIVE);
    truco_match_destroy(draw_match);

    struct truco_match *match = truco_match_create();
    assert(match != NULL);
    assert(truco_match_winner(match) == TRUCO_MATCH_ACTIVE);
    assert(truco_match_score(match, TRUCO_TEAM_ZERO) == 0);
    assert(truco_match_start_hand(match, TRUCO_HAND_ELEVEN_ACCEPT) ==
           TRUCO_HAND_START_ERROR);
    assert(truco_match_start_hand(match, TRUCO_HAND_ELEVEN_NONE) ==
           TRUCO_HAND_STARTED);
    assert(truco_match_hand_stake(match) == 1);
    assert(!truco_match_request_raise(match, 0, 1));
    assert(truco_match_request_raise(match, 0, 0));
    assert(truco_match_has_pending_raise(match));
    assert(!truco_match_request_raise(match, 0, 0));
    assert(!truco_match_respond_raise(match, TRUCO_TEAM_ZERO, true));
    assert(truco_match_respond_raise(match, TRUCO_TEAM_ONE, true));
    assert(truco_match_hand_stake(match) == 3);

    int challenger = TRUCO_TEAM_ONE;
    unsigned int stake = 3;
    while (stake < 12) {
        assert(truco_match_request_raise(match, (size_t)challenger,
                                         (size_t)challenger));
        assert(truco_match_respond_raise(match, 1 - challenger, true));
        stake = next_stake(stake);
        challenger = 1 - challenger;
        assert(truco_match_hand_stake(match) == stake);
    }
    assert(!truco_match_request_raise(match, (size_t)challenger,
                                      (size_t)challenger));
    assert(truco_match_finish_hand(match, TRUCO_TEAM_ONE));
    assert(truco_match_score(match, TRUCO_TEAM_ONE) == 12);
    assert(truco_match_winner(match) == TRUCO_TEAM_ONE);
    assert(truco_match_start_hand(match, TRUCO_HAND_ELEVEN_NONE) ==
           TRUCO_HAND_START_ERROR);
    assert(truco_match_score(NULL, TRUCO_TEAM_ZERO) == TRUCO_MATCH_INVALID);
    truco_match_destroy(match);

    test_raise_refusal_at(1);
    test_raise_refusal_at(3);
    test_raise_refusal_at(6);
    test_raise_refusal_at(9);

    match = truco_match_create();
    assert(match != NULL);
    score_team_to(match, TRUCO_TEAM_ONE, 10);
    assert(truco_match_start_hand(match, TRUCO_HAND_ELEVEN_NONE) ==
           TRUCO_HAND_STARTED);
    const int callers[] = {TRUCO_TEAM_ZERO, TRUCO_TEAM_ONE, TRUCO_TEAM_ZERO};
    for (size_t step = 0; step < 3; ++step) {
        const int caller = callers[step];
        assert(
            truco_match_request_raise(match, (size_t)caller, (size_t)caller));
        assert(truco_match_respond_raise(match, 1 - caller, true));
    }
    assert(truco_match_hand_stake(match) == 9);
    assert(truco_match_request_raise(match, TRUCO_TEAM_ONE, TRUCO_TEAM_ONE));
    assert(truco_match_respond_raise(match, TRUCO_TEAM_ZERO, false));
    assert(truco_match_score(match, TRUCO_TEAM_ONE) == 12);
    assert(truco_match_winner(match) == TRUCO_TEAM_ONE);
    truco_match_destroy(match);
}

static void test_hand_of_eleven_and_iron(void) {
    struct truco_match *match = truco_match_create();
    assert(match != NULL);
    score_team_to(match, TRUCO_TEAM_ZERO, 11);
    assert(truco_match_start_hand(match, TRUCO_HAND_ELEVEN_NONE) ==
           TRUCO_HAND_START_ERROR);
    assert(truco_match_start_hand(match, TRUCO_HAND_ELEVEN_ACCEPT) ==
           TRUCO_HAND_ELEVEN_ACCEPTED);
    assert(truco_match_hand_stake(match) == 3);
    assert(truco_match_finish_hand(match, TRUCO_TEAM_ZERO));
    assert(truco_match_score(match, TRUCO_TEAM_ZERO) == 12);
    assert(truco_match_winner(match) == TRUCO_TEAM_ZERO);
    truco_match_destroy(match);

    match = truco_match_create();
    assert(match != NULL);
    score_team_to(match, TRUCO_TEAM_ZERO, 10);
    score_team_to(match, TRUCO_TEAM_ONE, 10);
    assert(truco_match_start_hand(match, TRUCO_HAND_ELEVEN_NONE) ==
           TRUCO_HAND_STARTED);
    assert(truco_match_finish_hand(match, TRUCO_TEAM_ZERO));
    assert(truco_match_start_hand(match, TRUCO_HAND_ELEVEN_DECLINE) ==
           TRUCO_HAND_ELEVEN_DECLINED);
    assert(truco_match_score(match, TRUCO_TEAM_ONE) == 11);
    assert(truco_match_start_hand(match, TRUCO_HAND_ELEVEN_NONE) ==
           TRUCO_HAND_IRON_POLICY_REQUIRED);
    assert(!truco_match_hand_is_active(match));
    assert(!truco_match_resolve_iron_hand(match, TRUCO_TEAM_ZERO, 0));
    assert(truco_match_resolve_iron_hand(match, TRUCO_TEAM_ONE, 1));
    assert(truco_match_score(match, TRUCO_TEAM_ONE) == 12);
    assert(truco_match_winner(match) == TRUCO_TEAM_ONE);
    truco_match_destroy(match);

    match = truco_match_create();
    assert(match != NULL);
    score_team_to(match, TRUCO_TEAM_ZERO, 11);
    assert(truco_match_start_hand(match, TRUCO_HAND_ELEVEN_ACCEPT) ==
           TRUCO_HAND_ELEVEN_ACCEPTED);
    assert(truco_match_request_raise(match, 0, 0));
    assert(truco_match_winner(match) == TRUCO_TEAM_ONE);
    assert(truco_match_score(match, TRUCO_TEAM_ZERO) == 11);
    assert(truco_match_score(match, TRUCO_TEAM_ONE) == 0);
    truco_match_destroy(match);
}

static void test_explicit_iron_hand_policy(void) {
    struct truco_match *match = truco_match_create();
    assert(match != NULL);
    score_team_to(match, TRUCO_TEAM_ZERO, 10);
    score_team_to(match, TRUCO_TEAM_ONE, 8);
    assert(truco_match_start_hand(match, TRUCO_HAND_ELEVEN_NONE) ==
           TRUCO_HAND_STARTED);
    assert(truco_match_finish_hand(match, TRUCO_TEAM_ZERO));
    assert(truco_match_start_hand(match, TRUCO_HAND_ELEVEN_ACCEPT) ==
           TRUCO_HAND_ELEVEN_ACCEPTED);
    assert(truco_match_finish_hand(match, TRUCO_TEAM_ONE));
    assert(truco_match_score(match, TRUCO_TEAM_ZERO) == 11);
    assert(truco_match_score(match, TRUCO_TEAM_ONE) == 11);
    assert(!truco_match_start_iron_hand(match, 0));
    assert(!truco_match_start_iron_hand(match, 13));
    assert(truco_match_start_iron_hand(match, 1));
    assert(truco_match_hand_stake(match) == 1);
    assert(!truco_match_request_raise(match, 0, 0));
    assert(truco_match_finish_hand(match, TRUCO_TEAM_ZERO));
    assert(truco_match_score(match, TRUCO_TEAM_ZERO) == 12);
    assert(truco_match_winner(match) == TRUCO_TEAM_ZERO);
    truco_match_destroy(match);
}

static void test_coordinated_hand_and_raise(void) {
    struct truco_match *match = truco_match_create();
    struct truco_card_list *hands[TRUCO_PLAYER_COUNT];
    assert(match != NULL);
    for (size_t player = 0; player < TRUCO_PLAYER_COUNT; ++player) {
        hands[player] = truco_card_list_create();
        assert(hands[player] != NULL);
        for (size_t trick = 0; trick < 3; ++trick) {
            struct truco_card card = {(enum truco_rank)((player + trick) % 10),
                                      (enum truco_suit)player};
            assert(truco_card_list_append(hands[player], card));
        }
    }
    assert(truco_match_start_hand(match, TRUCO_HAND_ELEVEN_NONE) ==
           TRUCO_HAND_STARTED);
    struct truco_hand *hand = truco_hand_create(hands, match, TRUCO_THREE, 0);
    assert(hand != NULL);
    assert(truco_hand_trick_number(hand) == 1);
    struct truco_card card;
    assert(truco_card_list_get(hands[0], 0, &card));
    assert(truco_hand_play(hand, card));
    assert(truco_hand_current_player(hand) == 1);
    assert(truco_hand_request_raise(hand, 1));
    assert(truco_hand_has_pending_raise(hand));
    assert(truco_card_list_get(hands[1], 0, &card));
    assert(!truco_hand_play(hand, card));
    assert(truco_hand_respond_raise(hand, TRUCO_TEAM_ZERO, true));
    assert(truco_hand_play(hand, card));
    while (!truco_hand_is_complete(hand)) {
        const size_t player = truco_hand_current_player(hand);
        assert(player < TRUCO_PLAYER_COUNT);
        assert(truco_card_list_get(hands[player], 0, &card));
        assert(truco_hand_play(hand, card));
    }
    const int winner = truco_hand_winner(hand);
    assert(winner == TRUCO_TEAM_ZERO || winner == TRUCO_TEAM_ONE ||
           winner == TRUCO_HAND_DRAW);
    assert(truco_hand_trick_number(hand) == 2);
    assert(truco_match_score(match, TRUCO_TEAM_ZERO) +
               truco_match_score(match, TRUCO_TEAM_ONE) ==
           (winner == TRUCO_HAND_DRAW ? 0 : 3));
    assert(truco_hand_current_player(hand) == TRUCO_PLAYER_COUNT);
    truco_hand_destroy(hand);
    truco_match_destroy(match);
    for (size_t player = 0; player < TRUCO_PLAYER_COUNT; ++player) {
        truco_card_list_destroy(hands[player]);
    }
}

static void test_early_resolution_after_tied_trick(void) {
    const struct truco_card cards[TRUCO_PLAYER_COUNT][3] = {
        {{TRUCO_THREE, TRUCO_DIAMONDS},
         {TRUCO_FOUR, TRUCO_DIAMONDS},
         {TRUCO_QUEEN, TRUCO_SPADES}},
        {{TRUCO_THREE, TRUCO_SPADES},
         {TRUCO_FIVE, TRUCO_DIAMONDS},
         {TRUCO_JACK, TRUCO_SPADES}},
        {{TRUCO_THREE, TRUCO_HEARTS},
         {TRUCO_SIX, TRUCO_DIAMONDS},
         {TRUCO_KING, TRUCO_SPADES}},
        {{TRUCO_THREE, TRUCO_CLUBS},
         {TRUCO_SEVEN, TRUCO_DIAMONDS},
         {TRUCO_ACE, TRUCO_SPADES}},
    };
    struct truco_card_list *hands[TRUCO_PLAYER_COUNT];
    for (size_t player = 0; player < TRUCO_PLAYER_COUNT; ++player) {
        hands[player] = truco_card_list_create();
        assert(hands[player] != NULL);
        for (size_t card = 0; card < 3; ++card) {
            assert(truco_card_list_append(hands[player], cards[player][card]));
        }
    }
    struct truco_match *match = truco_match_create();
    assert(match != NULL);
    assert(truco_match_start_hand(match, TRUCO_HAND_ELEVEN_NONE) ==
           TRUCO_HAND_STARTED);
    struct truco_hand *hand = truco_hand_create(hands, match, TRUCO_TWO, 0);
    assert(hand != NULL);
    for (size_t trick = 0; trick < 2; ++trick) {
        for (size_t player = 0; player < TRUCO_PLAYER_COUNT; ++player) {
            const size_t current = truco_hand_current_player(hand);
            struct truco_card card;
            assert(current < TRUCO_PLAYER_COUNT);
            assert(truco_card_list_get(hands[current], 0, &card));
            assert(truco_hand_play(hand, card));
        }
        if (trick == 0) {
            assert(!truco_hand_is_complete(hand));
        }
    }
    assert(truco_hand_is_complete(hand));
    assert(truco_hand_winner(hand) == TRUCO_TEAM_ONE);
    assert(truco_hand_trick_number(hand) == 2);
    assert(truco_match_score(match, TRUCO_TEAM_ONE) == 1);
    truco_hand_destroy(hand);
    truco_match_destroy(match);
    for (size_t player = 0; player < TRUCO_PLAYER_COUNT; ++player) {
        truco_card_list_destroy(hands[player]);
    }
}

static void test_hidden_cards_and_partner_signals(void) {
    const struct truco_card cards[TRUCO_PLAYER_COUNT][3] = {
        {{TRUCO_FOUR, TRUCO_DIAMONDS},
         {TRUCO_SIX, TRUCO_DIAMONDS},
         {TRUCO_FOUR, TRUCO_SPADES}},
        {{TRUCO_FOUR, TRUCO_SPADES},
         {TRUCO_THREE, TRUCO_SPADES},
         {TRUCO_FIVE, TRUCO_SPADES}},
        {{TRUCO_FOUR, TRUCO_HEARTS},
         {TRUCO_SEVEN, TRUCO_DIAMONDS},
         {TRUCO_SIX, TRUCO_HEARTS}},
        {{TRUCO_FOUR, TRUCO_CLUBS},
         {TRUCO_FIVE, TRUCO_DIAMONDS},
         {TRUCO_SEVEN, TRUCO_CLUBS}},
    };
    struct truco_card_list *hands[TRUCO_PLAYER_COUNT];
    for (size_t player = 0; player < TRUCO_PLAYER_COUNT; ++player) {
        hands[player] = truco_card_list_create();
        assert(hands[player] != NULL);
        for (size_t index = 0; index < 3; ++index) {
            assert(truco_card_list_append(hands[player], cards[player][index]));
        }
    }
    struct truco_match *match = truco_match_create();
    assert(match != NULL);
    assert(truco_match_start_hand(match, TRUCO_HAND_ELEVEN_NONE) ==
           TRUCO_HAND_STARTED);
    struct truco_hand *hand = truco_hand_create(hands, match, TRUCO_THREE, 0);
    assert(hand != NULL);

    assert(truco_hand_signal(hand, 0, 257) == false);
    assert(truco_hand_signal(hand, 0, 42));
    unsigned int code = 0;
    assert(!truco_hand_partner_signal(hand, 0, &code));
    assert(!truco_hand_partner_signal(hand, 1, &code));
    struct truco_card card;
    assert(truco_card_list_get(hands[0], 0, &card));
    assert(!truco_hand_play_hidden(hand, card));
    assert(truco_card_list_size(hands[0]) == 3);
    for (size_t player = 0; player < TRUCO_PLAYER_COUNT; ++player) {
        assert(truco_hand_current_player(hand) == player);
        assert(truco_card_list_get(hands[player], 0, &card));
        assert(truco_hand_play(hand, card));
        if (player == 1) {
            assert(truco_hand_current_player(hand) == 2);
            assert(truco_hand_partner_signal(hand, 2, &code) && code == 42);
        }
    }
    assert(!truco_hand_is_complete(hand));
    assert(truco_hand_trick_number(hand) == 2);
    for (size_t player = 0; player < TRUCO_PLAYER_COUNT; ++player) {
        assert(truco_hand_current_player(hand) == player);
        assert(truco_card_list_get(hands[player], 0, &card));
        if (player == 1) {
            assert(truco_hand_play_hidden(hand, card));
            assert(truco_hand_trick_play_was_hidden(hand, 2, 1));
        } else {
            assert(truco_hand_play(hand, card));
        }
    }
    assert(truco_hand_is_complete(hand));
    assert(truco_hand_winner(hand) == TRUCO_TEAM_ZERO);
    assert(truco_round_create_for_trick(hands, TRUCO_THREE, 0, 0) == NULL);
    struct truco_round *all_hidden =
        truco_round_create_for_trick(hands, TRUCO_THREE, 0, 2);
    assert(all_hidden != NULL);
    for (size_t player = 0; player < TRUCO_PLAYER_COUNT; ++player) {
        assert(truco_round_current_player(all_hidden) == player);
        assert(truco_card_list_get(hands[player], 0, &card));
        assert(truco_round_play_hidden(all_hidden, card));
        assert(truco_round_play_was_hidden(all_hidden, player));
    }
    int trick_winner;
    assert(truco_round_winner(all_hidden, &trick_winner));
    assert(trick_winner == TRUCO_HAND_DRAW);
    truco_round_destroy(all_hidden);
    truco_hand_destroy(hand);
    truco_match_destroy(match);
    for (size_t player = 0; player < TRUCO_PLAYER_COUNT; ++player) {
        truco_card_list_destroy(hands[player]);
    }
}

static void test_hand_raise_refusal_and_eleven_loss(void) {
    struct truco_card_list *hands[TRUCO_PLAYER_COUNT];
    for (size_t player = 0; player < TRUCO_PLAYER_COUNT; ++player) {
        hands[player] = truco_card_list_create();
        assert(hands[player] != NULL);
        for (size_t trick = 0; trick < 3; ++trick) {
            struct truco_card card = {(enum truco_rank)((player + trick) % 10),
                                      (enum truco_suit)player};
            assert(truco_card_list_append(hands[player], card));
        }
    }

    struct truco_match *match = truco_match_create();
    assert(match != NULL);
    assert(truco_match_start_hand(match, TRUCO_HAND_ELEVEN_NONE) ==
           TRUCO_HAND_STARTED);
    struct truco_hand *hand = truco_hand_create(hands, match, TRUCO_THREE, 0);
    assert(hand != NULL);
    assert(truco_hand_request_raise(hand, 0));
    assert(truco_hand_respond_raise(hand, TRUCO_TEAM_ONE, false));
    assert(truco_hand_is_complete(hand));
    assert(truco_hand_winner(hand) == TRUCO_TEAM_ZERO);
    assert(truco_match_score(match, TRUCO_TEAM_ZERO) == 1);
    truco_hand_destroy(hand);
    truco_match_destroy(match);

    match = truco_match_create();
    assert(match != NULL);
    score_team_to(match, TRUCO_TEAM_ZERO, 11);
    assert(truco_match_start_hand(match, TRUCO_HAND_ELEVEN_ACCEPT) ==
           TRUCO_HAND_ELEVEN_ACCEPTED);
    hand = truco_hand_create(hands, match, TRUCO_THREE, 0);
    assert(hand != NULL);
    assert(truco_hand_request_raise(hand, 0));
    assert(truco_hand_is_complete(hand));
    assert(truco_hand_winner(hand) == TRUCO_TEAM_ONE);
    assert(truco_match_winner(match) == TRUCO_TEAM_ONE);
    assert(truco_match_score(match, TRUCO_TEAM_ZERO) == 11);
    truco_hand_destroy(hand);
    truco_match_destroy(match);
    for (size_t player = 0; player < TRUCO_PLAYER_COUNT; ++player) {
        truco_card_list_destroy(hands[player]);
    }
}

int main(void) {
    test_match_wagers();
    test_hand_of_eleven_and_iron();
    test_explicit_iron_hand_policy();
    test_coordinated_hand_and_raise();
    test_early_resolution_after_tied_trick();
    test_hidden_cards_and_partner_signals();
    test_hand_raise_refusal_and_eleven_loss();
    struct truco_card_list *first = truco_standard_deck_create();
    struct truco_card_list *same_seed = truco_standard_deck_create();
    struct truco_card_list *other_seed = truco_standard_deck_create();
    assert(first != NULL && same_seed != NULL && other_seed != NULL);
    assert(truco_card_list_size(first) == 40);
    for (size_t i = 0; i < 40; ++i) {
        struct truco_card card;
        assert(truco_card_list_get(first, i, &card));
        assert(card.rank <= TRUCO_THREE);
        assert(card.suit <= TRUCO_CLUBS);
        for (size_t j = 0; j < i; ++j) {
            struct truco_card previous;
            assert(truco_card_list_get(first, j, &previous));
            assert(card.rank != previous.rank || card.suit != previous.suit);
        }
    }
    assert(truco_deck_shuffle(first, 1234));
    assert(truco_deck_shuffle(same_seed, 1234));
    assert(truco_deck_shuffle(other_seed, 4321));
    assert_same_deck(first, same_seed);
    bool seen[40] = {false};
    for (size_t i = 0; i < 40; ++i) {
        struct truco_card card;
        assert(truco_card_list_get(first, i, &card));
        const size_t key = (size_t)card.rank * 4 + (size_t)card.suit;
        assert(key < 40 && !seen[key]);
        seen[key] = true;
    }
    bool differs = false;
    for (size_t i = 0; i < 40; ++i) {
        struct truco_card left;
        struct truco_card right;
        assert(truco_card_list_get(first, i, &left));
        assert(truco_card_list_get(other_seed, i, &right));
        differs |= left.rank != right.rank || left.suit != right.suit;
    }
    assert(differs);

    assert(truco_manilha_rank(TRUCO_THREE) == TRUCO_FOUR);
    assert(truco_manilha_rank(TRUCO_SEVEN) == TRUCO_QUEEN);
    struct truco_card clubs = {TRUCO_QUEEN, TRUCO_CLUBS};
    struct truco_card hearts = {TRUCO_QUEEN, TRUCO_HEARTS};
    struct truco_card spades = {TRUCO_QUEEN, TRUCO_SPADES};
    struct truco_card diamonds = {TRUCO_QUEEN, TRUCO_DIAMONDS};
    assert(truco_card_compare(clubs, hearts, TRUCO_QUEEN) > 0);
    assert(truco_card_compare(hearts, spades, TRUCO_QUEEN) > 0);
    assert(truco_card_compare(spades, diamonds, TRUCO_QUEEN) > 0);
    struct truco_card three = {TRUCO_THREE, TRUCO_DIAMONDS};
    struct truco_card four = {TRUCO_FOUR, TRUCO_CLUBS};
    assert(truco_card_compare(four, three, TRUCO_FOUR) > 0);
    struct truco_card two = {TRUCO_TWO, TRUCO_DIAMONDS};
    struct truco_card ace = {TRUCO_ACE, TRUCO_CLUBS};
    struct truco_card king = {TRUCO_KING, TRUCO_CLUBS};
    struct truco_card jack = {TRUCO_JACK, TRUCO_CLUBS};
    struct truco_card queen = {TRUCO_QUEEN, TRUCO_CLUBS};
    struct truco_card seven = {TRUCO_SEVEN, TRUCO_CLUBS};
    struct truco_card six = {TRUCO_SIX, TRUCO_CLUBS};
    struct truco_card five = {TRUCO_FIVE, TRUCO_CLUBS};
    assert(truco_card_compare(three, two, TRUCO_THREE) > 0);
    assert(truco_card_compare(two, ace, TRUCO_THREE) > 0);
    assert(truco_card_compare(ace, king, TRUCO_THREE) > 0);
    assert(truco_card_compare(king, jack, TRUCO_THREE) > 0);
    assert(truco_card_compare(jack, queen, TRUCO_THREE) > 0);
    assert(truco_card_compare(queen, seven, TRUCO_THREE) > 0);
    assert(truco_card_compare(seven, six, TRUCO_THREE) > 0);
    assert(truco_card_compare(six, five, TRUCO_THREE) > 0);
    assert(truco_card_compare(five, four, TRUCO_THREE) > 0);

    struct truco_card_list *hands[TRUCO_PLAYER_COUNT];
    for (size_t player = 0; player < TRUCO_PLAYER_COUNT; ++player) {
        hands[player] = truco_card_list_create();
        assert(hands[player] != NULL);
    }
    const struct truco_card played[] = {
        {TRUCO_QUEEN, TRUCO_CLUBS},
        {TRUCO_THREE, TRUCO_CLUBS},
        {TRUCO_QUEEN, TRUCO_DIAMONDS},
        {TRUCO_QUEEN, TRUCO_HEARTS},
    };
    for (size_t player = 0; player < TRUCO_PLAYER_COUNT; ++player) {
        assert(truco_card_list_append(hands[player], played[player]));
    }
    struct truco_card_list *duplicate_hands[TRUCO_PLAYER_COUNT] = {
        hands[0], hands[0], hands[2], hands[3]};
    assert(truco_round_create(duplicate_hands, TRUCO_QUEEN, 0) == NULL);
    struct truco_round *round = truco_round_create(hands, TRUCO_QUEEN, 2);
    assert(round != NULL);
    assert(truco_round_current_player(round) == 2);
    assert(!truco_round_is_complete(round));
    int winner = 99;
    assert(!truco_round_winner(round, &winner));
    assert(!truco_round_play(round, played[1]));
    assert(truco_round_current_player(round) == 2);
    const size_t turn_order[] = {2, 3, 0, 1};
    for (size_t turn = 0; turn < TRUCO_PLAYER_COUNT; ++turn) {
        assert(truco_round_current_player(round) == turn_order[turn]);
        assert(truco_round_play(round, played[turn_order[turn]]));
    }
    assert(truco_round_is_complete(round));
    assert(truco_round_current_player(round) == TRUCO_PLAYER_COUNT);
    assert(!truco_round_play(round, played[0]));
    assert(truco_round_winner(round, &winner) && winner == 0);
    assert(truco_card_list_size(truco_round_played_cards(round)) == 4);
    assert(truco_card_list_size(hands[0]) == 0);
    truco_round_destroy(round);
    for (size_t player = 0; player < TRUCO_PLAYER_COUNT; ++player) {
        truco_card_list_destroy(hands[player]);
    }

    for (size_t player = 0; player < TRUCO_PLAYER_COUNT; ++player) {
        hands[player] = truco_card_list_create();
        assert(hands[player] != NULL);
    }
    const struct truco_card tied_cards[] = {
        {TRUCO_THREE, TRUCO_DIAMONDS},
        {TRUCO_THREE, TRUCO_HEARTS},
        {TRUCO_TWO, TRUCO_SPADES},
        {TRUCO_ACE, TRUCO_CLUBS},
    };
    for (size_t player = 0; player < TRUCO_PLAYER_COUNT; ++player) {
        assert(truco_card_list_append(hands[player], tied_cards[player]));
    }
    round = truco_round_create(hands, TRUCO_QUEEN, 0);
    assert(round != NULL);
    for (size_t player = 0; player < TRUCO_PLAYER_COUNT; ++player) {
        assert(truco_round_current_player(round) == player);
        assert(truco_round_play(round, tied_cards[player]));
    }
    assert(truco_round_winner(round, &winner) && winner == -1);
    truco_round_destroy(round);
    for (size_t player = 0; player < TRUCO_PLAYER_COUNT; ++player) {
        truco_card_list_destroy(hands[player]);
    }

    for (size_t player = 0; player < TRUCO_PLAYER_COUNT; ++player) {
        hands[player] = truco_card_list_create();
        assert(hands[player] != NULL);
    }
    const struct truco_card tie_then_manilha[] = {
        {TRUCO_THREE, TRUCO_DIAMONDS},
        {TRUCO_THREE, TRUCO_HEARTS},
        {TRUCO_QUEEN, TRUCO_CLUBS},
        {TRUCO_FOUR, TRUCO_SPADES},
    };
    for (size_t player = 0; player < TRUCO_PLAYER_COUNT; ++player) {
        assert(truco_card_list_append(hands[player], tie_then_manilha[player]));
    }
    round = truco_round_create(hands, TRUCO_QUEEN, 0);
    assert(round != NULL);
    for (size_t player = 0; player < TRUCO_PLAYER_COUNT; ++player) {
        assert(truco_round_play(round, tie_then_manilha[player]));
    }
    assert(truco_round_winner(round, &winner) && winner == 2);
    truco_round_destroy(round);
    for (size_t player = 0; player < TRUCO_PLAYER_COUNT; ++player) {
        truco_card_list_destroy(hands[player]);
    }

    const int tie_winners[3] = {-1, -1, -1};
    const int first_tie_then_second[3] = {-1, 1, 0};
    const int first_then_tie[3] = {2, -1, 1};
    const int third_tie[3] = {0, 1, -1};
    const int all_three[3] = {0, 2, 1};
    const int invalid_result[3] = {0, 4, 1};
    assert(truco_resolve_hand(tie_winners) == -1);
    assert(truco_resolve_hand(first_tie_then_second) == 1);
    const int first_two_tied_then_player_three[3] = {-1, -1, 3};
    assert(truco_resolve_hand(first_two_tied_then_player_three) == 1);
    assert(truco_resolve_hand(first_then_tie) == 0);
    assert(truco_resolve_hand(third_tie) == 0);
    assert(truco_resolve_hand(all_three) == 0);
    assert(truco_resolve_hand(invalid_result) == -2);
    assert(truco_resolve_hand(NULL) == -2);

    truco_card_list_destroy(first);
    truco_card_list_destroy(same_seed);
    truco_card_list_destroy(other_seed);
    return 0;
}
