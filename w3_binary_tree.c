// binary tree
//
#include <stdio.h>
#include <stdlib.h>

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

Node* init_node(Data data_val, int level, Node *left, Node *right)
{
    Node *node;
    node = malloc(sizeof(Node));
    node->data = data_val;
    node->level = level;
    node->left = left;
    node->right = right;
    return node;
}

Node *create_tree(Data *data, int i, int size)
{
    int level;
    if (i >= size)
        return NULL;
    else
        // level = int(ln2(i))
        level = i;
        return init_node(data[i], level,
                         create_tree(data, 2*i+1, size),
                         create_tree(data, 2*i+2, size));
}

void main() {
    char data[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g'};
    // int num = 1000000;
    // int data[num];
    // for (int i = 0; i < num; i++)
    //   data[i] = 10*i;
    Node *b;
    int num=4;
    b = create_tree(data, 0, num);
    inorder(b);
    printf("\n");
    postorder(b);
    printf("\n");
}
