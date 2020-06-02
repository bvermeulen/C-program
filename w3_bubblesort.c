// sort numbers with a recursive bubble sort
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUMPERLINE 5
#define SIZE       100

typedef struct node
{
    int val;
    struct node *next;
} Node;

int is_empty(Node *node)
{
    return node == NULL;
}

int count(Node *node, int i)
{
  if (is_empty(node)) return i;
  else count(node->next, i+1);
}

void _print_list(Node *node, int i)
// print list recursively and wrap line every 10 numbers
//
{
    if (is_empty(node)) printf("\n----------------------------\n");
    else
    {
        printf("%6i, ", node->val);
        if (i % NUMPERLINE == 0)
            printf("\n");
        i++;
        _print_list(node->next, i);
    }
}

void print_list(Node *node)
{
  _print_list(node, 1);
}

Node *create_node(const int val, Node *next)
{
    Node *node;
    node = malloc(sizeof(Node));
    node->val = val;
    node->next = next;
    return node;
}

Node *create_list_rand(int i, int size)
// create list recursively with random numbers
{
    if (i >= size) return NULL;
    else
    {
        int val = 1 + (rand() % size);
        i++;
        return create_node(val, create_list_rand(i, size));
    }
}

Node *create_list_from_array(int *a, int i, int size)
// create list recursively from array a
{
    if (i >= size) return NULL;
    else
    {
        int val = a[i];
        i++;
        return create_node(val, create_list_from_array(a, i, size));
    }
}

void swap(Node *h1, Node *h2)
// swap the values between two nodes
{
    int val = h2->val;
    h2->val = h1->val;
    h1->val = val;
}

void bubble(Node *node, int num)
// bubble the highest value to the end of the list recursively
// only do this (num - 1) times as there are num-1 comparisons
{
    if (num == 0) return;
    else
    {
        if (node->val > node->next->val)
        {
            swap(node, node->next);
        }
        bubble(node->next, num - 1);
    }
}

void _bubble_sort(Node *node, Node *head, int num)
// sort the list recursively by calling bubble num times
// note head of list is preserved in the function as each time
// the list has to be traversed from the start with one node less
{
    if (num == 0) return;
    else
    {
        bubble(node, num - 1);
        node = head;
        _bubble_sort(node, head, num - 1);
    }
}

void bubble_sort(Node *node)
{
  _bubble_sort(node, node, count(node, 0));
}

void main()
{
    Node *list;
    int size;
    srand(time(0));

    // test bubblesort with an array
    printf("Unsorted list ...\n");
    int a[] = {13, 3, 8, 14, 2, 6, 4, 12, 11, 10, 0, 5};
    size = sizeof(a) / sizeof(a[0]);
    list = create_list_from_array(a, 0, size);
    print_list(list);

    printf("List sorted ...\n");
    bubble_sort(list);
    print_list(list);

    // do bubble sort with a list of random numbers
    printf("Unsorted list random numbers ...\n");
    size = SIZE;
    list = create_list_rand(0, size);
    print_list(list);

    printf("List sorted ...\n");
    bubble_sort(list);
    print_list(list);
}
