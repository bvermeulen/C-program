// binary tree
//
#include <stdio.h>
#include <stdlib.h>

#define NUM_NODES 100

typedef char Data;
typedef struct node {
    Data data;
    int level;
    struct node *left;
    struct node *right;
} Node;

void inorder(Node *root)
{
    char cleft = '-', cright = '-';
    if (root != NULL) {
        inorder(root->left);
        if (root->left != NULL) cleft = (root->left)->data;
        if (root->right != NULL) cright = (root->right)->data;
        printf("(%i: %p: %c [%p: %c, %p: %c) \n",
          root->level, root, root->data, root->left, cleft, root->right, cright);
        inorder(root->right);
    }
}
void memoryorder(unsigned long int pointers[], int size)
{
  Node *head;

  char cleft = '-', cright = '-';
  for (int i = 0; i < size; i++)
  {
    head = pointers[i];
    printf("=======> head %p ==>", head);
    if (head->left != NULL) cleft = (head->left)->data;
    if (head->right != NULL) cright = (head->right)->data;

    printf("(%i: %p: %c [%p: %c, %p: %c) \n",
      head->level, head, head->data, head->left, cleft, head->right, cright);

  }
}

void postorder(Node *root)
{
    char cleft = '-', cright = '-';
    if (root != NULL) {
        if (root->left != NULL) cleft = (root->left)->data;
        if (root->right != NULL) cright = (root->right)->data;
        printf("(%i: %p: %c [%p: %c, %p: %c) \n",
        root->level, root, root->data, root->left, cleft, root->right, cright);
        postorder(root -> left);
        postorder(root -> right);
    }
}

void init_node(Data data_val, int i, Node **nodes, int size)
{
    nodes[i]->data = data_val;
    nodes[i]->level = i;
    nodes[i]->left = NULL;
    nodes[i]->right = NULL;
    if (2*i + 1 < size)
      nodes[i]->left = nodes[2*i+1];
    if (2*i + 2 < size)
      nodes[i]->right = nodes[2*i+2];
}

Node  *build_tree(Data *data, int size)
{
    Node *nodes[size];
    for (int i=0; i < size; i++) nodes[i] = malloc(sizeof(Node));
    for (int i=0; i < size; i++) init_node(data[i], i, nodes, size);

    return nodes[0];
}

Node *build_tree_recursive(Data *data, int i, int size, unsigned long int pointers[], int *j)
{
    if (i >= size)
        return NULL;
    else
    {
        Node *head = malloc(sizeof(Node));
        pointers[*j] = head;
        (*j)++;
        head->data = data[i];
        head->level = i;
        head->left = build_tree_recursive(data, 2*i + 1, size, pointers, j);
        head->right = build_tree_recursive(data, 2*i + 2, size, pointers, j);
        return head;
    }
}

void main() {
  /*
    Tree with 8 nodes: order h, d, b, e, a, f, c, g
                            a
                        b       c
                     d    e   f   g
                  h

  */
    char data[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
    Node *head, *head_recursive;
    int num=8, j=0;
    unsigned long int pointers[num];
    head = build_tree(data, num);
    // inorder(head);
    // printf("\n");
    // postorder(head);
    // memoryorder(head, num);
    // printf("-------------------------\n");

    head_recursive = build_tree_recursive(data, 0, num, pointers, &j);
    inorder(head_recursive);
    printf("\n");
    // postorder(head_recursive);
    memoryorder(pointers, num);
    printf("\n");
}
