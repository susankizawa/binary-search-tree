#ifndef BST_H
#define BST_H

typedef struct Node{
    int value;
    int height;
    struct Node* parent;
    struct Node* left;
    struct Node* right;
} Node;

Node* findMin(Node* root);

Node* findMax(Node* root);

int getHeightNonRecur(Node* root);

int getHeight(Node* root);

void updateHeight(Node* node);

int getBalanceFactor(Node* root);

Node* createNode(int value);

Node* findNode(Node* root, int value);

void printNode(Node* n);

Node* insertNodeNonRecur(Node* root, int value);

Node* insertNode(Node* root, int value);

Node* removeNodeNonRecur(Node* root, int value);

Node* removeNode(Node* root, int value);

Node* rotateLeft(Node* p);

Node* rotateRight(Node* p);

Node* doubleRotateLeft(Node* root);

Node* doubleRotateRight(Node* root);

Node* balanceTree(Node* root);

void preOrderPrintNonRecur(Node* root);

void preOrderPrint(Node* root);

void inOrderPrintNonRecur(Node* root);

void inOrderPrint(Node* root);

void postOrderPrintNonRecur(Node* root);

void postOrderPrint(Node* root);

void levelOrderPrint(Node* root);

#endif // BST_H
