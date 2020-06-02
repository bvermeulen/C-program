// Open and read a file of integer pairs into an array that is created with the first integer telling you how many to read.
//
// So  4 9  11  4  5    would mean create an integer array size 4 and read into data[].
//
// Write a routine that places these values into a binary tree structure. Then walk the tree “inorder” and print these values to the screen.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXNUMBERS   100

typedef struct Node {
    int value;
    struct Node *left;
    struct Node *right;
} Node;

Node *create_node(int value)
{
    Node *node = malloc(sizeof(Node));
    node->value = value;
    node->left = NULL;
    node->right = NULL;
}

void print_inorder(Node *head)
// print in order - first left nodes then right nodes
{
    int leftVal = -1, rightVal = -1;
    if (head != NULL)
    {
        // left nodes first
        print_inorder(head->left);
        if (head->left != NULL)
            leftVal = (head->left)->value;

        if (head->right != NULL)
            rightVal = (head->right)->value;

        printf("(A-%p: %2d [L-%p: %2d, R-%p: %2d) \n",
               head, head->value, head->left, leftVal, head->right, rightVal);

        // then the right node
        print_inorder(head->right);
    }
}

Node *create_tree(int *data, int size)
// create tree non recursively
{
    Node *nodes[size];

    // create all the nodes needed, one for each element of data
    for (int i = 0; i < size; i++)
    {
        nodes[i] = create_node(data[i]);
    }

    // ... now make the links for left and then right
    for (int i = 0; i < size; i++)
    {
        if (2*i + 1 < size)
            nodes[i]->left = nodes[2*i + 1];
        if (2*i + 2 < size)
            nodes[i]->right = nodes[2*i + 2];
    }
    // return the first node
    return nodes[0];
}

Node *create_tree_recursive(int *data, int i, int size)
// create tree recursively, first explore left then right
{
    if (i >= size)
        return NULL;
    else
    {
        Node *node = create_node(data[i]);
        node->left = create_tree_recursive(data, 2 * i + 1, size);
        node->right = create_tree_recursive(data, 2 * i + 2, size);
        return node;
    }
}

void read_numbers(char *fileName, int *data, int *size)
// read file with integer numbers separated by space, first number is size of list
{
    FILE *file_ptr;
    file_ptr = fopen(fileName, "r");
    rewind(file_ptr);

    fscanf(file_ptr, "%d", &(*size));
    for (int i = 0; i < *size; i++)
        fscanf(file_ptr, "%d", &data[i]);
}

int main(int argc, char *argv[])
{
    // default file name is "w4_4_numbers.txt"
    char fileName[] = "w4_4_numbers.txt";
    int data[MAXNUMBERS];
    int size;
    Node *head;

    if (argc != 2)
        printf("Default file name \"%s\" is used ...\n", fileName);
    else
    {
        strcpy(fileName, argv[1]);
        printf("File name \"%s\" is used ...\n", fileName);
    }

    read_numbers(fileName, data, &size);

    printf("Tree made recursively ...\n");
    head = create_tree_recursive(data, 0, size);
    print_inorder(head);

    printf("-----------------------------\n");
    printf("Tree made non recursively ...\n");
    head = create_tree(data, size);
    print_inorder(head);

    return 0;
}
