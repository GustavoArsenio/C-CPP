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
                             enum truco_rank manilha_rank) {
  const size_t player = truco_hand_current_player(hand);
  if (player >= TRUCO_PLAYER_COUNT ||
      truco_card_list_size(hands[player]) == 0) {
    return false;
  }
  struct truco_card selected;
  if (!truco_card_list_get(hands[player], 0, &selected)) {
    return false;
  }
  for (size_t index = 1; index < truco_card_list_size(hands[player]); ++index) {
    struct truco_card candidate;
    if (!truco_card_list_get(hands[player], index, &candidate)) {
      return false;
    }
    if (truco_card_compare(candidate, selected, manilha_rank) < 0) {
      selected = candidate;
    }
  }
  return truco_hand_play(hand, selected);
}

static void destroy_hands(struct truco_card_list *hands[TRUCO_PLAYER_COUNT]) {
  for (size_t player = 0; player < TRUCO_PLAYER_COUNT; ++player) {
    truco_card_list_destroy(hands[player]);
  }
}

static bool play_automatic_hand(struct truco_match *match, uint32_t seed,
                                size_t hand_number, bool is_iron_hand) {
  struct truco_card_list *deck = truco_standard_deck_create();
  struct truco_card_list *hands[TRUCO_PLAYER_COUNT] = {NULL};
  struct truco_hand *hand = NULL;
  bool success = false;
  if (deck == NULL || !truco_deck_shuffle(deck, seed)) {
    goto cleanup;
  }
  for (size_t player = 0; player < TRUCO_PLAYER_COUNT; ++player) {
    hands[player] = truco_card_list_create();
    if (hands[player] == NULL) {
      goto cleanup;
    }
  }

  struct truco_card vira;
  if (!deal_hand(deck, hands, &vira)) {
    goto cleanup;
  }
  const enum truco_rank manilha_rank = truco_manilha_rank(vira.rank);
  hand = truco_hand_create(hands, match, manilha_rank, 0);
  if (hand == NULL) {
    goto cleanup;
  }
  printf("hand=%zu kind=%s vira=%s/%s manilha=%s stake=%u\n", hand_number,
         is_iron_hand ? "iron" : "normal", truco_rank_name(vira.rank),
         truco_suit_name(vira.suit), truco_rank_name(manilha_rank),
         truco_match_hand_stake(match));
  while (!truco_hand_is_complete(hand)) {
    if (!play_lowest_card(hand, hands, manilha_rank)) {
      goto cleanup;
    }
  }
  printf("hand_winner=%d score=%d-%d\n", truco_hand_winner(hand),
         truco_match_score(match, TRUCO_TEAM_ZERO),
         truco_match_score(match, TRUCO_TEAM_ONE));
  success = true;

cleanup:
  truco_hand_destroy(hand);
  destroy_hands(hands);
  truco_card_list_destroy(deck);
  return success;
}

int main(int argc, char **argv) {
  if (argc > 2) {
    fprintf(stderr, "Usage: auto-match <seed>\n");
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

  struct truco_match *match = truco_match_create();
  if (match == NULL) {
    return 1;
  }
  const uint32_t initial_seed = (uint32_t)parsed_seed;
  uint32_t hand_seed = initial_seed;
  size_t hand_number = 0;
  bool success = true;
  printf("seed=%" PRIuMAX "\n", parsed_seed);
  printf(
      "Policy: weakest card, no raises, accept at 11; iron hand awards "
      "one point; player 0 leads each hand and trick.\n");

  while (truco_match_winner(match) == TRUCO_MATCH_ACTIVE) {
    const bool both_at_eleven =
        truco_match_score(match, TRUCO_TEAM_ZERO) == 11 &&
        truco_match_score(match, TRUCO_TEAM_ONE) == 11;
    if (both_at_eleven) {
      if (!truco_match_start_iron_hand(match, 1)) {
        success = false;
        break;
      }
      ++hand_number;
      if (!play_automatic_hand(match, hand_seed, hand_number, true)) {
        success = false;
        break;
      }
      hand_seed += UINT32_C(0x9e3779b9);
      continue;
    }
    enum truco_hand_eleven_choice choice = TRUCO_HAND_ELEVEN_NONE;
    if (truco_match_score(match, TRUCO_TEAM_ZERO) == 11 ||
        truco_match_score(match, TRUCO_TEAM_ONE) == 11) {
      choice = TRUCO_HAND_ELEVEN_ACCEPT;
    }
    const enum truco_hand_start_result started =
        truco_match_start_hand(match, choice);
    if (started == TRUCO_HAND_IRON_POLICY_REQUIRED) {
      fprintf(stderr, "This automatic policy has no iron-hand decision.\n");
      success = false;
      break;
    }
    if (started != TRUCO_HAND_STARTED &&
        started != TRUCO_HAND_ELEVEN_ACCEPTED) {
      success = false;
      break;
    }
    ++hand_number;
    if (!play_automatic_hand(match, hand_seed, hand_number, false)) {
      success = false;
      break;
    }
    hand_seed += UINT32_C(0x9e3779b9);
  }

  if (success) {
    printf("match_winner=%d final_score=%d-%d hands=%zu\n",
           truco_match_winner(match), truco_match_score(match, TRUCO_TEAM_ZERO),
           truco_match_score(match, TRUCO_TEAM_ONE), hand_number);
  }
  truco_match_destroy(match);
  return success ? 0 : 1;
}
