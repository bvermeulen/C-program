/*
 *
 * Use a struct to define a card as an enumerated member that is its suit value and a short
 * that is its pips value.
 *
 * Write a function that randomly shuffles the deck.
 *
 * Then deal out 7 card hands and evaluate the probability that a hand has no pair, one pair,
 * two pair, three of a kind, full house and 4 of a kind.  This is a Monte Carlo method to get an
 * approximation to these probabilities.  Use at least 1 million randomly generated hands.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#define NUMCARDS      52
#define CARDS_HAND    7

enum CardSuit {Clubs, Diamonds, Hearts, Spades};
enum CardPip {c2, c3, c4, c5, c6, c7, c8, c9, c10, Jack, Queen, King, Ace};

typedef struct Card {
    enum CardSuit suit;
    enum CardPip pip;
} Card;

typedef struct Results
{
    int no_hand;
    int pair;
    int two_pair;
    int three_of_a_kind;
    int full_house;
    int four_of_a_kind;
} Results;

void get_namecard(char namecard[], const enum CardSuit suit, const enum CardPip pip) {
    /* get namecard stings composed from suit and pip of the card
     */
    char *namesuit;
    char *namepip;

    switch(suit) {
        case Clubs: namesuit = "Clubs"; break;
        case Diamonds: namesuit = "Diamonds"; break;
        case Hearts: namesuit = "Hearts"; break;
        case Spades: namesuit = "Spades"; break;
        default: namesuit = "Unknown suit";
    }

    switch(pip) {
        case c2: namepip = "2"; break;
        case c3: namepip = "3"; break;
        case c4: namepip = "4"; break;
        case c5: namepip = "5"; break;
        case c6: namepip = "6"; break;
        case c7: namepip = "7"; break;
        case c8: namepip = "8"; break;
        case c9: namepip = "9"; break;
        case c10: namepip = "10"; break;
        case Jack: namepip = "Jack"; break;
        case Queen: namepip = "Queen"; break;
        case King: namepip = "King"; break;
        case Ace: namepip = "Ace"; break;
        default: namepip = "Unknown pip";
    }

    strcpy(namecard, "");
    strcat(namecard, namesuit); strcat(namecard, "-"); strcat(namecard, namepip);
}

void print_card(const Card card) {
    char namecard[14];
    get_namecard(namecard, card.suit, card.pip);
    printf("card: %s\n", namecard);
}

void create_cards(Card cards[], const int print_on) {
    /* Create set of 52 cards 4 suits x 13 pips
     */
    if (print_on) printf("New deck - not shuffled\n");

    int cardnum = 0;
    for (int suit = 0; suit < 4; suit++) {
        for (int pip = 0; pip < 13; pip++) {
            cards[cardnum].suit = suit;
            cards[cardnum].pip = pip;
            if (print_on) print_card(cards[cardnum]);
            cardnum++;
        }
    }
    if (print_on) printf("\n");
}

void shuffle_deck(const Card cards[], Card cards_shuffled[], const int print_on) {
    /* Loop over the deck and randomnaly choose a card from stack of remaining cards,
     * then swap the current card with the one randomly selected and
     * continue until the last card.
     */
    int r;
    Card card;
    if (print_on) printf("Deck - shuffled\n");

    // copy cards to shuffled cards
    for (int i = 0; i < NUMCARDS; i++) {
        cards_shuffled[i] = cards[i];
    }

    // ... and shuffle
    for (int i = 0; i < NUMCARDS; i++) {
        r = i + rand() % (NUMCARDS - i);
        card = cards_shuffled[r];
        cards_shuffled[r] = cards_shuffled[i];
        cards_shuffled[i] = card;
        if (print_on) print_card(cards_shuffled[i]);
    }
    if (print_on) printf("\n");
}

void deal_cards(const int players, const Card cards[NUMCARDS], Card cards_hand[players][CARDS_HAND]) {
    /* Deal alternating cards depending on number of players. No more than 5 players
     */
    int cardnum = 0;
    assert(players < 6);

    for (int card = 0; card < CARDS_HAND; card++) {
        for (int player = 0; player < players; player++) {
            cards_hand[player][card] = cards[cardnum];
            cardnum++;
        }
    }
}

Results result_hand(const int *pipcount, const int pips) {
    /* Determine the result of the hand by looping over the pips that have
     * or more occurances. Add the count to the enumerated type Result and
     * return result.
     */
    int count_pairs = 0;
    int count_three = 0;
    int count_four = 0;
    Results result;

    result.no_hand = 0;
    result.pair = 0;
    result.two_pair = 0;
    result.three_of_a_kind = 0;
    result.full_house = 0;
    result.four_of_a_kind = 0;

    for (int i = 0; i < pips; i++) {
        switch (pipcount[i]) {
        case 2: count_pairs++; break;
        case 3: count_three++; break;
        case 4: count_four++; break;
        default: break;
        }
    }

    if (count_four == 1) result.four_of_a_kind++;
    else if (count_three == 2) result.full_house++;
    else if ((count_three == 1) && (count_pairs > 0)) result.full_house++;
    else if (count_three == 1) result.three_of_a_kind++;
    else if (count_pairs > 1) result.two_pair++;
    else if (count_pairs == 1) result.pair++;
    else result.no_hand++;

    return result;
}

Results analyse_hand(Card cardhand[CARDS_HAND]) {
    /* Analyze the hand by looping over the cards in hand and check if the pip of the
     * first card reoccur in the remaining cards. These pips are stored in an array
     * pip. Pips that are not the same are stored in an arrany newhand for the next
     * loop. The number of different pips in the hand are the variable pips.
     * Return the function result_hand, that will return a struct Result
     */
    enum CardPip hand[7], newhand[6], pip[7];
    int pipcount[7], pipindex, sizehand, handindex, cardindex;
    int pips;
    Results result;

    // copy cardhand to hand to keep cardhand intact
    for (int i = 0; i < CARDS_HAND; i++) {
        hand[i] = cardhand[i].pip;
    }

    cardindex = 0;
    sizehand = CARDS_HAND;
    while (sizehand > 0) {
        pipindex = 0;
        handindex = 0;
        for (int j = 0; j < sizehand; j++) {
            if (hand[0] == hand[j]) {
                pip[pipindex] = hand[j];
                pipindex++;
            }
            else {
                newhand[handindex] = hand[j];
                handindex++;
            }
        }
        pipcount[cardindex] = pipindex;

        cardindex++;
        // create a new hand from newhand
        sizehand = handindex;
        for (int k = 0; k < sizehand; k++)
        {
            hand[k] = newhand[k];
        }
    }
    return result_hand(pipcount, cardindex);
}

void test_hand(Card hand[CARDS_HAND]) {
    enum CardSuit suit[CARDS_HAND] = {Clubs, Diamonds, Hearts, Spades, Clubs, Diamonds, Hearts};
    enum CardPip pip[CARDS_HAND] = {King, King, Queen, c2, c3, Queen, c4};
    for (int i = 0; i < CARDS_HAND; i++) {
        hand[i].suit = suit[i];
        hand[i].pip = pip[i];
        print_card(hand[i]);
    }
}

void main() {
    int players = 3;
    Card cards[NUMCARDS], cards_shuffled[NUMCARDS];
    Card cards_hand[players][CARDS_HAND], hand[CARDS_HAND];
    Results result, total_result;
    srand(time(0));

    // create a deck of 52 cards and shuffle
    create_cards(cards, 1);

    // Check a know hand of cards
    printf("Test a known hand\n");
    test_hand(hand);
    result = analyse_hand(hand);
    if (result.four_of_a_kind) printf("Four of kind");
    else if (result.full_house) printf("Full House");
    else if (result.three_of_a_kind) printf("Three of a kind");
    else if (result.two_pair) printf("Two pairs");
    else if (result.pair) printf("One pair");
    else if (result.no_hand) printf("No hand");
    else printf("Check code not a valid option ...");
    printf("\n\n");

    // shuffle and deal cards
    shuffle_deck(cards, cards_shuffled, 1);
    deal_cards(players, cards_shuffled, cards_hand);
    for (int player = 0; player < players; player++) {
        printf("Player %i has following cards:\n", player + 1);
        for (int card = 0; card < CARDS_HAND; card++) {
            print_card(cards_hand[player][card]);
        }

        printf("  ==> ", player);
        // analyse the hand and print the results
        for (int i = 0; i < CARDS_HAND; i++) {
            hand[i] = cards_hand[player][i];
        }

        result = analyse_hand(hand);
        if (result.four_of_a_kind) printf("Four of kind");
        else if (result.full_house) printf("Full House");
        else if (result.three_of_a_kind) printf("Three of a kind");
        else if (result.two_pair) printf("Two pairs");
        else if (result.pair) printf("One pair");
        else if (result.no_hand) printf("No hand");
        else printf("Check code not a valid option ...");
        printf("\n\n");
    }

    // Do some statistics
    int rounds = 1000000;
    total_result.four_of_a_kind = 0;
    total_result.full_house = 0;
    total_result.three_of_a_kind = 0;
    total_result.two_pair = 0;
    total_result.pair = 0;
    total_result.no_hand = 0;

    for (int i = 0; i < rounds; i++) {
        shuffle_deck(cards, cards_shuffled, 0);
        deal_cards(players, cards_shuffled, cards_hand);

        // Analyse the hand of player 1
        for (int k = 0; k < CARDS_HAND; k++) {
            hand[k] = cards_hand[0][k];
        }
        result = analyse_hand(hand);
        if (result.four_of_a_kind) total_result.four_of_a_kind++;
        else if (result.full_house) total_result.full_house++;
        else if (result.three_of_a_kind) total_result.three_of_a_kind++;
        else if (result.two_pair) total_result.two_pair++;
        else if (result.pair) total_result.pair++;
        else if (result.no_hand) total_result.no_hand++;
        else printf("Check code not a valid option ...");
    }

    // .. and print the results
    float prob_four_of_a_kind = 0.00168067;
    float prob_full_house = 0.02596102;
    float prob_three_of_a_kind = 0.04829870;
    float prob_two_pair = 0.23495536;
    float prob_pair = 0.43822546;
    float prob_no_hand = 0.17411920;
    float prob_r_four_of_a_kind, prob_r_full_house, prob_r_three_of_a_kind, prob_r_two_pair, prob_r_pair, prob_r_no_hand;
    float diff_four_of_a_kind, diff_full_house, diff_three_of_a_kind, diff_two_pair, diff_pair, diff_no_hand;

    prob_r_four_of_a_kind = (float) total_result.four_of_a_kind / (float) rounds;
    prob_r_full_house = (float) total_result.full_house / (float) rounds;
    prob_r_three_of_a_kind = (float) total_result.three_of_a_kind / (float) rounds;
    prob_r_two_pair = (float) total_result.two_pair / (float) rounds;
    prob_r_pair = (float) total_result.pair / (float) rounds;
    prob_r_no_hand = (float) total_result.no_hand / (float) rounds;

    diff_four_of_a_kind = prob_four_of_a_kind - prob_r_four_of_a_kind;
    diff_full_house = prob_full_house - prob_r_full_house;
    diff_three_of_a_kind = prob_three_of_a_kind - prob_r_three_of_a_kind;
    diff_two_pair = prob_two_pair - prob_r_two_pair;
    diff_pair = prob_pair - prob_r_pair;
    diff_no_hand = prob_no_hand - prob_r_no_hand;

    printf("Statistics for %i rounds\n", rounds);
    printf("Four of a kind : %i (%.6f, %.6f)\n", total_result.four_of_a_kind, prob_r_four_of_a_kind, diff_four_of_a_kind);
    printf("Full house     : %i (%.6f, %.6f)\n", total_result.full_house, prob_r_full_house, diff_full_house);
    printf("Three of a kind: %i (%.6f, %.6f)\n", total_result.three_of_a_kind, prob_r_three_of_a_kind, diff_three_of_a_kind);
    printf("Two pair       : %i (%.6f, %.6f)\n", total_result.two_pair, prob_r_two_pair, diff_two_pair);
    printf("Pair           : %i (%.6f, %.6f)\n", total_result.pair, prob_r_pair, diff_pair);
    printf("No hand        : %i (%.6f, %.6f)\n", total_result.no_hand, prob_r_no_hand, diff_no_hand);
    printf("\nNote that deviations (especially pairs and no hand) are because I do not (yet) check for flush and straignts!\n");
}
