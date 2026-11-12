# Assignment Requirements Summary

This summary records the requirements reviewed in the preserved PDFs. The PDFs
remain authoritative where the summary is abbreviated.

## Lista 1

Required exercises (all integer input):

1. Sum 18 integers.
2. Average 18 integers.
3. Read 4 integers; report the mean of odd values, largest even value, and
   largest-minus-smallest difference.
4. Average 100 integers.
5. Read two arrays of 50 and produce pairwise means.
6. Read array A of 30 and produce B with each value multiplied by 3.
7. For 100 values, multiply evens by 5 and add 5 to odds.
8. Read arrays A and B of 20 and exchange corresponding values.
9. Concatenate arrays A and B of 15 into C of 30.

The source also lists six complementary exercises: maximum of two numbers,
maximum of a 20-value vector, factorial, square side for half an input square's
area, shortest path visiting three cities once, and maximum of three numbers.
These six complementary exercises are now represented as planned catalog records; implementation remains pending.

## Lista 2

- Array Fill I (URI 1173): input integer V < 50; fill ten entries by doubling;
  print `N[i] = X`.
- Fibonacci Array: read T cases; each N is 0–60; print `Fib(N) = X`; values fit
  unsigned 64-bit integers.
- Array Fill III: read a double with four decimal places; output 100 values,
  halving each time and printing four decimal places.
- Array Fill IV: read 15 integers into even (`par`) and odd (`impar`) buffers,
  each of five positions. Print and reuse each full buffer immediately; print
  remaining odd values before remaining even values.
- Line in Array: read L (0–11), S/M, and 144 floating-point values. For row L,
  operate on the green region from the PDF figure. For L=2, the pictured region
  selects columns 3 through 8 inclusive. Output one decimal place.
- Below the Main Diagonal: use only cells with row > column from a 12x12
  floating-point matrix; sum or average; print one decimal place. There are 66
  selected cells.
- Square Matrix I (URI 1435): read sizes 1–100 until zero, create the
  concentric-layer pattern, width-three right-aligned cells, and a blank line
  after each matrix.
- Square Matrix III (URI 1557): read sizes 1–15 until zero; cell (i,j) is
  2^(i+j); field width is the digit count of the largest value; blank line
  after each matrix.

## Trabalho 1: Truco Paulista

The assignment asks for a linked-list-based card model and a playable game.
The deck has 40 cards (exclude 8, 9, 10 and jokers), four players receive three
cards, and the turned-up `vira` determines the manilhas. Card rank order from
strongest to weakest is manilhas (suits in order clubs, hearts, spades,
diamonds), 3, 2, ace, king, jack, queen, 7, 6, 5, 4. The document includes
special manilha successor examples for vira 3 and 7.

A hand consists of up to three rounds. Players alternate between partnerships.
A match ends when a partnership reaches 12 points. A hand is worth 1 point and
raises may progress through 3, 6, 9, and 12; refusing awards the challenger the
previous stake. At 11 points, the partnership may inspect partner cards and
accept a 3-point hand; declining awards the opponents one point. Asking for
Truco while at 11 points causes immediate defeat. The PDF defines tie resolution
by prior rounds, with three tied rounds awarding no points. Hiding a card is
allowed as a bluff except in the first round. Players may signal their partner.

Required program features include shuffling, dealing, automatic play, and
user-selected card play. Required structures include a list for shuffling,
per-player card lists, an undealt-card pile, a played-card pile, and a circular
list for alternating player order. Hiding a played card as a bluff is allowed
except in the first trick; partner signaling is also allowed.

The 40-card linked-list deck, seeded shuffle, four three-card hands, vira,
manilha successor, card comparison, circular four-player turn order, card
transfer to the played pile, trick winner/tie detection, three-trick hand tie
resolution, stake transitions, scorekeeping and hand-of-eleven decisions now
have tested implementations. A hand coordinator now connects up to three trick
results to the wager and score engine, ending after two when those results
already determine the winner; a pending raise blocks card play until
the opposing partnership responds. The match engine caps score at 12. A Truco
request by a team at 11 records an opponent victory without changing score,
since the PDF gives no score adjustment. When both teams are at 11, the generic
engine requires the caller to supply an explicit iron-hand point policy; it
does not silently invent those rules. The deterministic automatic match
demonstrates one explicit frontend policy: accept at 11, play an iron hand for
one point, never raise, play the weakest available card, and use player 0 as
leader. That policy is specific to the simulator and is not attributed to the
assignment. A user-controlled four-seat terminal mode now shares the same
state transitions for card selection, face-up or face-down plays, opaque
partner signals, raises, hand-of-eleven choices, and an explicit iron-hand
award. At 11 it reveals the teammate's hand before the accept/decline choice.
The four seats share a terminal and no private display is guaranteed. The
frontend pairs seats 0/2 as team 0 and seats 1/3 as team 1, so it reveals seat 2
or seat 3 as the corresponding partner.

The PDF permits a face-down bluff but does not state its effect on trick
ranking. This implementation's explicit policy counts a face-down play toward
completion of the trick but excludes its card from ranking; if all four plays
are face down, the trick is a draw. A caller can inspect the internal played
pile, so a frontend must use hidden-play metadata and must not print the card
value. The PDF permits partner signals without specifying a vocabulary or
meaning. The engine therefore carries an opaque 0–255 code from a player to
their partner, without interpreting it. The latest signal for that team
persists for the hand. The shared terminal displays the code and provides no
privacy guarantees; a private signaling interface needs a separate frontend.

Drawn hands can extend a match beyond twelve hands; the score reaching twelve,
not a fixed hand count, ends the automatic match.

The PDF also leaves unclear whether the vira changes per hand or per trick,
hand-of-iron acceptance/stake, partner signal semantics, and starting-player
choice. The hand API receives the leader from
its caller and keeps that leader for all tricks; both frontends choose player
0. The automatic weakest-card strategy is illustrative, not optimal. The
automatic iron-hand point award and the interactive user's award are frontend
policies.

## Lista 4: Linked Lists

Transcribed by visual review of the three scanned pages. The assignment lists
12 exercises:

1. `int comprimento(Lista *l)`: count nodes in an integer singly linked list.
2. `int maiores(Lista *l, int x)`: count nodes whose `info` value is greater
   than x.
3. `int ultimo(Lista *l)`: return the last integer value.
4. `void concatena(Lista *l1, Lista *l2)`: append real-valued list 2 to list 1
   and leave list 2 empty.
5. `void retira_n(Lista *l, int x)`: remove every occurrence of x.
6. `Lista* separa(Lista* l, int x)`: split after the first occurrence of x;
   the returned list contains the suffix after x, while x remains in the first
   list. Return the created list even when empty.
7. `Lista* merge(Lista* l1, Lista* l2)`: interleave nodes from two real-valued
   lists into a newly allocated list, transferring ownership and leaving both
   input lists empty. Append any excess nodes in order.
8. `void inverte(Lista* l)`: reverse the links in a list.
9. `Lista* copia(Lista* l)`: create a copy of a character list, with iterative
   and recursive implementations.
10. `void para_circular(Lista* l)`: convert a singly linked list to a circular
    list.
11. `retira_inicio` and `retira_final`: remove first and last elements from a
    doubly linked list.
12. Implement insertion and removal for a circular doubly linked list.

Exercises 1, 2, 3, 5, 6, and 8 use the canonical opaque integer-list API.
Exercises 4 and 7 use a separate opaque double-list API that transfers nodes
without copying payloads. Exercise 9 has iterative and recursive deep-copy
implementations for character lists. Exercise 10 converts a singly linked
integer list to circular form. Exercise 11 removes both ends of a doubly linked
integer list; exercise 12 inserts and removes by index in a circular doubly
linked integer list. These APIs are implemented in
`data-structures/linear/lista4_lists.{h,c}` and covered by `unit_lista4_lists`.
