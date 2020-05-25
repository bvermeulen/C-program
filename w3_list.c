// Week 3 Honors exercise
// Modify the singly linked list to be a doubly linked list. Now write a routine that
// removes all duplicate data in the doubly linked list. The data will be integers
// generated at random from [0,49]. Initially have a list of 200 elements.
//
// Now do this in one of two ways.  Sort the list by its data field. Remove adjacent
// elements of the sorted list with the same value. Or, take the first element—search
// the remainder of the list for elements with the same data and remove them. Then take
// the second element and do the same. Repeat until only one element is left.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE       200
#define RANGE      50
#define NUMPERLINE 10

typedef struct Node {
    int value;
    struct Node *next;
} Node;

typedef struct Node_dl {
    int value;
    struct Node_dl  *prev;
    struct Node_dl  *next;

} Node_dl;

int is_empty(Node *h) {
    return (h == NULL);
}

int is_empty_dl(Node_dl *h) {
    return (h == NULL);
}

Node *create_node(const int value) {
    Node *node = malloc(sizeof(Node));
    node->value = value;
    node->next = NULL;
    return node;
}

Node_dl *create_node_dl(const int value) {
    Node_dl *node = malloc(sizeof(Node_dl));
    node->value = value;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

void print_list(Node *h, int i) {
    if (is_empty(h)) printf("\n------------------------\n");
    else {
        printf("%3d, ", h->value);
        if ((i % NUMPERLINE) == 0) printf("\n");
        print_list(h->next, i+1);
    }
}

void print_extended(Node *h)
// print extended node information, include address and next pointer
{
    if (is_empty(h)) printf("\n------------------------\n");
    else {
        printf("[(%10p : %3d) | %10p]\n", h, h->value, h->next);
        print_extended(h->next);
    }
}

void print_list_dl(Node_dl *h, int i) {
    if (is_empty_dl(h)) printf("\n------------------------\n");
    else {
        printf("%3d, ", h->value);
        if ((i % NUMPERLINE) == 0) printf("\n");
        print_list_dl(h->next, i+1);
    }
}

void print_extended_dl(Node_dl *h)
// print extended node information, include address and next, prev pointers
{
    if (is_empty_dl(h)) printf("\n------------------------\n");
    else {
        printf("[%10p | (%10p : %3d) | %10p]\n", h->prev, h, h->value, h->next);
        print_extended_dl(h->next);
    }
}

Node *create_random_list(int i, int size, Node *head)
// create single linked list of length size with random integer numbers in range
// [0...RANGE-1]
{
    Node *new_node;
    int value;
    if (i >= size) return head;
    else {
        value = rand() % RANGE;
        new_node = create_node(value);
        new_node->next = head;
        head = create_random_list(i+1, size, new_node);
    }
}

Node_dl *convert_list_to_dl(Node *head, Node_dl *head_dl)
// convert single linked list (head) to a double linked list (head_dl),
// note that values of head_dl are in reversed order as head.
{
    int value;
    Node_dl *new_node;
    if (is_empty(head)) return head_dl;
    else {
        value = head->value;
        new_node = create_node_dl(value);
        if (!is_empty_dl(head_dl)) head_dl->prev = new_node;
        new_node->next = head_dl;
        head_dl = convert_list_to_dl(head->next, new_node);
    }
}

void *delete_duplicates(Node_dl *head)
// delete duplicates in double linked list by running a double loop. Delete
// duplicate nodes by realigning the pointers of two nodes that are neighbours of
// the node to be removed.
{
    Node_dl *h2;
    while (!is_empty_dl(head)) {
        h2 = head->next;
        while (!is_empty_dl(h2)) {
            if (h2->value == head->value) {
                h2->prev->next = h2->next;
                if (!is_empty_dl(h2->next)) h2->next->prev = h2->prev;
            }
            h2 = h2->next;
        }
        head = head->next;
    }
}

int main() {
    srand(time(0));

    // create a single linked list with random numbers in range [0 ... RANGE-1]
    Node *head = create_random_list(0, SIZE, NULL);
    printf("Single linked list ...\n");
    print_list(head, 1);
    // print_extended(head);          // commented out

    // convert single linked list to a double linked list
    Node_dl *head_dl = convert_list_to_dl(head, NULL);
    printf("Double linked list ... \n");
    print_list_dl(head_dl, 1);
    // print_extended_dl(head_dl);    // commented out

    // Delete duplicate and print list
    delete_duplicates(head_dl);
    printf("Same list with duplicates removed ...\n");
    print_list_dl(head_dl, 1);
    // print_extended_dl(head_dl);    // commented out

    return 0;
}
