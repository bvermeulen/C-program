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

typedef enum CardSuit {Clubs, Diamonds, Hearts, Spades} CardSuit;
typedef enum CardPip {c2, c3, c4, c5, c6, c7, c8, c9, c10, Jack, Queen, King, Ace} CardPip;

typedef struct Card {
    CardSuit suit;
    CardPip pip;
} Card;

typedef struct Result_flush {
    int is_flush;
    CardSuit flush;
} Result_flush;

typedef struct Result_straight {
    int is_straight;
    CardPip start;
    int is_flush;
} Result_straight;

typedef struct Result_pips {
    int numpips;
    CardPip pip[CARDS_HAND];
    int count[CARDS_HAND];
} Result_pips;


typedef struct Results {
    int royal_flush;
    int straight_flush;
    int four_of_a_kind;
    int full_house;
    int flush;
    int straight;
    int three_of_a_kind;
    int two_pair;
    int pair;
    int no_hand;
} Results;

void get_namecard(char namecard[], const CardSuit suit, const CardPip pip) {
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

void sort_hand(Card *hand) {
    /* sort the hand on pip from low to high
     */
    int i, j, minidx;
    CardPip tmp_pip;
    CardSuit tmp_suit;

    for (i = 0; i < CARDS_HAND - 1; i++)
    {
        minidx = i;
        for (j = i + 1; j < CARDS_HAND; j++)
        {
            if (hand[j].pip < hand[minidx].pip)
                minidx = j;
        }
        // swap the vals
        tmp_pip = hand[minidx].pip;
        tmp_suit = hand[minidx].suit;
        hand[minidx].pip = hand[i].pip;
        hand[minidx].suit = hand[i].suit;
        hand[i].pip = tmp_pip;
        hand[i].suit = tmp_suit;
    }
}

Result_flush check_flush(const Card *hand) {
    int count_clubs = 0, count_diamonds = 0, count_hearts = 0, count_spades = 0;
    Result_flush result;

    for (int i=0; i < CARDS_HAND; i++) {
        switch (hand[i].suit) {
            case Clubs: count_clubs++; break;
            case Diamonds: count_diamonds++; break;
            case Hearts: count_hearts++; break;
            case Spades: count_spades++; break;
            default: break;
        }
    }

    if (count_clubs > 4) {
        result.flush = Clubs;
        result.is_flush = 1;
    }
    else if (count_diamonds > 4) {
        result.flush = Diamonds;
        result.is_flush = 1;
    }
    else if (count_hearts > 4) {
        result.flush = Hearts;
        result.is_flush = 1;
    }
    else if (count_spades > 4) {
        result.flush = Spades;
        result.is_flush = 1;
    }
    else {
        result.flush = 4;
        result.is_flush = 0;
    }

    return result;
}

Result_straight check_straight(const Card *cardhand, const Result_flush flush) {
    int i, j, index, found, sizehand, count, result_count, last_card_flag;
    CardPip start;
    Card hand[CARDS_HAND];
    Card playhand[CARDS_HAND];
    Result_straight result;

    // copy the array
    for (i = 0; i < CARDS_HAND; i++) {
        hand[i].suit = cardhand[i].suit;
        hand[i].pip = cardhand[i].pip;
    }
    sort_hand(hand);

    // remove cards with same pip, pips must be sorted from low to high
    index = 0;
    i = 0;
    last_card_flag = 0;
    while (i < CARDS_HAND - 1) {
        found = 0;
        if (hand[i].pip != hand[i+1].pip) {
            playhand[index].pip = hand[i].pip;
            playhand[index].suit = hand[i].suit;
            i++;
        }
        else {
            // check same cards keep the one with a suit same as flush
            j = i;
            while (hand[j].pip == hand[j+1].pip && j < CARDS_HAND - 1) {
                if (hand[j+1].suit == flush.flush) {
                    playhand[index].pip = hand[j+1].pip;
                    playhand[index].suit = hand[j+1].suit;
                    found = 1;
                    if (j+1 == CARDS_HAND-1) last_card_flag = 1;
                }
                j++;
            }
            // if no card is found with same suit as flush then just
            // add the first one
            if (!found) {
                playhand[index].pip = hand[i].pip;
                playhand[index].suit = hand[i].suit;
            }
            i = j+1;
        }
        index++;
    }
    // handle last card unless it is handled before
    if (!last_card_flag) {
        playhand[index].pip = hand[i].pip;
        playhand[index].suit = hand[i].suit;
        index++;
    }

    // now find the straights in playhand
    i = 0;
    result.is_straight = 0;
    result.is_flush = 1;
    sizehand = index;
    while (i < sizehand - 1) {
        count = 1;
        for (j = i; j < sizehand; j++) {
            if (playhand[j+1].pip - playhand[j].pip == 1) {
                count++;
                if (playhand[j+1].suit != playhand[j].suit) {
                   result.is_flush = 0;
                }
            }
            else {
                // straigt found and capture the results
                if (count > 4) {
                    result.is_straight = 1;
                    start = playhand[i].pip;
                }
                // and continue to see if there is a straight starting at a
                // higher pip
                i = j + 1;
                break;
            }
        }
    }
    result.start = start;
    return result;
}

Result_pips check_pips(const Card *cardhand) {
    /* Determine the result of the hand by looping over the pips that have
     * or more occurances. Add the count to the enumerated type Result and
     * return result.
     */
    Card hand[CARDS_HAND], newhand[CARDS_HAND-1];
    int pipindex, sizehand, handindex, count;
    Result_pips result;

    // copy cardhand to hand to keep cardhand intact and determine if hand is a flush
    for (int i = 0; i < CARDS_HAND; i++) {
        hand[i].pip = cardhand[i].pip;
        hand[i].suit = cardhand[i].suit;
    }

    sizehand = CARDS_HAND;
    pipindex = 0;
    count = 0;
    while (sizehand > 0) {
        handindex = 0;
        result.pip[pipindex] = hand[0].pip;
        for (int j = 0; j < sizehand; j++) {
            if (hand[0].pip == hand[j].pip) {
                count++;
            }
            else {
                newhand[handindex].pip = hand[j].pip;
                newhand[handindex].suit = hand[j].suit;
                handindex++;

            }
        }
        result.count[pipindex] = count;
        count = 0;
        pipindex++;
        sizehand = handindex;
        // copy new hand to hand to count number of next pips
        for (int k = 0; k < handindex; k++) {
            hand[k] = newhand[k];
        }
    }
    result.numpips = pipindex;
    return result;
}

Results result_hand(const Result_flush flush, const Result_straight straight, const Result_pips pips) {
    int count_pairs = 0;
    int count_three = 0;
    int count_four = 0;
    Results result;

    result.royal_flush = 0;
    result.straight_flush = 0;
    result.four_of_a_kind = 0;
    result.full_house = 0;
    result.flush = 0;
    result.straight = 0;
    result.three_of_a_kind = 0;
    result.two_pair = 0;
    result.pair = 0;
    result.no_hand = 0;

    for (int i = 0; i < pips.numpips; i++) {
        switch (pips.count[i]) {
        case 2: count_pairs++; break;
        case 3: count_three++; break;
        case 4: count_four++; break;
        default: break;
        }
    }

    if (straight.is_straight && straight.start == c10 && straight.is_flush) result.royal_flush++;
    else if (straight.is_flush && straight.is_straight) result.straight_flush++;
    else if (count_four == 1) result.four_of_a_kind++;
    else if (count_three == 2) result.full_house++;
    else if ((count_three == 1) && (count_pairs > 0)) result.full_house++;
    else if (flush.is_flush) result.flush++;
    else if (straight.is_straight) result.straight++;
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
    Result_flush flush;
    Result_straight straight;
    Result_pips pips;
    Results result;

    // check for flush and straight
    flush = check_flush(cardhand);
    straight = check_straight(cardhand, flush);
    pips = check_pips(cardhand);
    return result_hand(flush, straight, pips);
}

void print_result(const Results result) {

    if (result.royal_flush) printf("Royal Flush");
    else if (result.straight_flush) printf("Straight Flush");
    else if (result.four_of_a_kind) printf("Four of kind");
    else if (result.full_house) printf("Full House");
    else if (result.flush) printf("Flush");
    else if (result.straight) printf("Straight");
    else if (result.three_of_a_kind) printf("Three of a kind");
    else if (result.two_pair) printf("Two pairs");
    else if (result.pair) printf("One pair");
    else if (result.no_hand) printf("No hand");
    else printf("Check code not a valid option ...");
    printf("\n\n");
}

void test_hand(Card hand[CARDS_HAND]) {
    // Flush
    // CardSuit suit[CARDS_HAND] = {Hearts, Diamonds, Hearts, Hearts, Clubs, Hearts, Hearts};
    // CardPip pip[CARDS_HAND] = {c2, King, c4, c5, c3, King, c6};

    // Straight
    CardSuit suit[CARDS_HAND] = {Clubs, Diamonds, Hearts, Hearts, Clubs, Hearts, Hearts};
    CardPip pip[CARDS_HAND] = {c2, King, c4, c5, c3, King, c6};

    // Straight Flush
    // CardSuit suit[CARDS_HAND] = {Clubs, Diamonds, Clubs, Clubs, Clubs, Clubs, Clubs};
    // CardPip pip[CARDS_HAND] = {c2, King, c4, c5, c3, King, c6};

    // Royal Flush
    // CardSuit suit[CARDS_HAND] = {Clubs, Diamonds, Clubs, Clubs, Clubs, Spades, Clubs};
    // CardPip pip[CARDS_HAND] = {King, c2, Queen, c10, Jack, Ace, Ace};

    // Pair
    // CardSuit suit[CARDS_HAND] = {Clubs, Diamonds, Hearts, Spades, Clubs, Diamonds, Hearts};
    // CardPip pip[CARDS_HAND] = {c2, King, c4, c8, c3, King, c6};

    // Mystery
    // CardSuit suit[CARDS_HAND] = {Clubs, Clubs, Spades, Hearts, Spades, Spades, Hearts};
    // CardPip pip[CARDS_HAND] = {c6, c4, c4, c4, c10, Queen, Jack};

    for (int i = 0; i < CARDS_HAND; i++) {
        hand[i].suit = suit[i];
        hand[i].pip = pip[i];
        print_card(hand[i]);
    }
}

void main() {
    int players = 3;
    int rounds = 3000000;
    Card cards[NUMCARDS], cards_shuffled[NUMCARDS];
    Card cards_hand[players][CARDS_HAND], hand[CARDS_HAND];
    Results result, total_result;
    srand(time(0));

    // create a deck of 52 cards and shuffle
    create_cards(cards, 0);

    // Check a know hand of cards
    printf("Test a known hand\n");
    test_hand(hand);
    result = analyse_hand(hand);
    print_result(result);

    // shuffle and deal cards
    shuffle_deck(cards, cards_shuffled, 0);
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
        print_result(result);
    }

    // Do some statistics
    total_result.royal_flush = 0;
    total_result.straight_flush = 0;
    total_result.four_of_a_kind = 0;
    total_result.full_house = 0;
    total_result.flush = 0;
    total_result.straight = 0;
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
        if (result.royal_flush) total_result.royal_flush++;
        else if (result.straight_flush) total_result.straight_flush++;
        else if (result.four_of_a_kind) total_result.four_of_a_kind++;
        else if (result.full_house) total_result.full_house++;
        else if (result.flush) total_result.flush++;
        else if (result.straight) total_result.straight++;
        else if (result.three_of_a_kind) total_result.three_of_a_kind++;
        else if (result.two_pair) total_result.two_pair++;
        else if (result.pair) total_result.pair++;
        else if (result.no_hand) total_result.no_hand++;
        else printf("Check code not a valid option ...");
    }

    // .. and print the results
    float p_royal_flush = 0.00003232;
    float p_straigh_flush = 0.0027851;
    float p_four_of_a_kind = 0.00168067;
    float p_full_house = 0.02596102;
    float p_flush = 0.03025494;
    float p_straight = 0.04619382;
    float p_three_of_a_kind = 0.04829870;
    float p_two_pair = 0.23495536;
    float p_pair = 0.43822546;
    float p_no_hand = 0.17411920;
    float p_r_royal_flush, p_r_straight_flush, p_r_four_of_a_kind, p_r_full_house;
    float p_r_flush, p_r_straight, p_r_three_of_a_kind, p_r_two_pair, p_r_pair;
    float p_r_no_hand;
    float d_royal_flush, d_straight_flush, d_four_of_a_kind, d_full_house;
    float d_flush, d_straight, d_three_of_a_kind, d_two_pair, d_pair, d_no_hand;

    p_r_royal_flush = (float) total_result.royal_flush / (float) rounds;
    p_r_straight_flush = (float) total_result.straight_flush / (float) rounds;
    p_r_four_of_a_kind = (float) total_result.four_of_a_kind / (float) rounds;
    p_r_full_house = (float) total_result.full_house / (float) rounds;
    p_r_flush = (float)total_result.flush / (float)rounds;
    p_r_straight = (float)total_result.straight / (float)rounds;
    p_r_three_of_a_kind = (float) total_result.three_of_a_kind / (float) rounds;
    p_r_two_pair = (float) total_result.two_pair / (float) rounds;
    p_r_pair = (float) total_result.pair / (float) rounds;
    p_r_no_hand = (float) total_result.no_hand / (float) rounds;

    d_royal_flush = p_royal_flush - p_r_royal_flush;
    d_straight_flush = p_straigh_flush - p_r_straight_flush;
    d_four_of_a_kind = p_four_of_a_kind - p_r_four_of_a_kind;
    d_full_house = p_full_house - p_r_full_house;
    d_flush = p_flush - p_r_flush;
    d_straight = p_straight - p_r_straight;
    d_three_of_a_kind = p_three_of_a_kind - p_r_three_of_a_kind;
    d_two_pair = p_two_pair - p_r_two_pair;
    d_pair = p_pair - p_r_pair;
    d_no_hand = p_no_hand - p_r_no_hand;

    printf("Statistics for %i rounds\n", rounds);
    printf("Royal Flush    : %i (%.6f, %.6f)\n", total_result.royal_flush, p_r_royal_flush, d_royal_flush);
    printf("Straight flush : %i (%.6f, %.6f)\n", total_result.straight_flush, p_r_straight_flush, d_straight_flush);
    printf("Four of a kind : %i (%.6f, %.6f)\n", total_result.four_of_a_kind, p_r_four_of_a_kind, d_four_of_a_kind);
    printf("Full house     : %i (%.6f, %.6f)\n", total_result.full_house, p_r_full_house, d_full_house);
    printf("Flush          : %i (%.6f, %.6f)\n", total_result.flush, p_r_flush, d_flush);
    printf("Straight       : %i (%.6f, %.6f)\n", total_result.straight_flush, p_r_straight, d_straight_flush);
    printf("Three of a kind: %i (%.6f, %.6f)\n", total_result.three_of_a_kind, p_r_three_of_a_kind, d_three_of_a_kind);
    printf("Two pair       : %i (%.6f, %.6f)\n", total_result.two_pair, p_r_two_pair, d_two_pair);
    printf("Pair           : %i (%.6f, %.6f)\n", total_result.pair, p_r_pair, d_pair);
    printf("No hand        : %i (%.6f, %.6f)\n", total_result.no_hand, p_r_no_hand, d_no_hand);
    printf("\ncheck value from \n");
}
