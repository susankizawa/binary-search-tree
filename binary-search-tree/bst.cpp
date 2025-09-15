#include "bst.h"

#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include "queue.h"
#include "stack.h"

Node* findMin(Node* root){
    if(root == NULL)
        return root;

    Node* current = root;

    while(1){
        if(current->left)
            current = current->left;
        else
            break;
    }

    return current;
}

Node* findMax(Node* root){
    if(root == NULL)
        return root;

    Node* current = root;

    while(1){
        if(current->right)
            current = current->right;
        else
            break;
    }

    return current;
}

int getHeightNonRecur(Node* root){
    if(root == NULL)
        return -1;

    Queue* q = createQueue();

    enqueue(q, root);
    int height = -1;

    while(!queueIsEmpty(q)){
        int levelSize = q->size;
        for(int i = 0; i < levelSize; i++){
            Node* current = dequeue(q);
            if(current->left)
                enqueue(q, current->left);
            if(current->right)
                enqueue(q, current->right);
        }
        height++;
    }

    free(q);

    return height;
}

int getHeight(Node* root){
    if(root == NULL)
        return -1;

    return 1 + std::max(getHeight(root->left), getHeight(root->right));
}

int getBalanceFactor(Node* root){
    if(root == NULL)
        return 0;

    return (getHeight(root->right) - getHeight(root->left));
}

Node* createNode(int value){
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->value = value;
    newNode->parent = NULL;
    newNode->left = newNode->right = NULL;
    return newNode;
}

Node* findNode(Node* root, int value){
    Node* current = root;

    while(current != NULL){
        if (value < current->value){
            current = current->left;
        } else if(value > current->value){
            current = current->right;
        } else{
            return current;
        }
    }

    // 404: Value was not found!
    return NULL;
}

void printNode(Node* n){
    if(n == NULL){
        printf("[ NULL ]");
        return;
    }

    printf("[ %d ]", n->value);
    return;
}

Node* insertNodeNonRecur(Node* root, int value){
    // create newNode with the defined value to be inserted in the tree
    Node* newNode = createNode(value);

    // if root is null, return newNode as the root of the new tree
    if(root == NULL)
        return newNode;

    // goes through the tree looking for a free spot for the new node
    Node* current = root;

    while(1){
        // if value is less than the current node's value move left
        // if it's less move right
        // if it's the same value, return the root of the tree without inserting
        if(value < current->value){
            // if left child exists, current is now the left child, else exit the loop
            if(current->left)
                current = current->left;
            else
                break;
        } else if(value > current->value){
            // if right child exists, current is now the right child, else exit the loop
            if(current->right)
                current = current->right;
            else
                break;
        } else {
            // frees newNode to have no duplicates
            free(newNode);
            return root;
        }
    }

    // Inserts new node in the free spot
    newNode->parent = current;
    if(value < current->value){
        current->left = newNode;
    } else {
        current->right = newNode;
    }

    return root;
}

Node* insertNode(Node* root, int value){
    // if root is NULL, we found the spot for the new node
    if(root == NULL){
        Node* newNode = createNode(value);
        return newNode;
    }

    // if value < root's value, go left
    // if value > root's value, go right
    // if value == root's value, do nothing (no duplicates)
    if(value < root->value){
        root->left = insertNode(root->left, value);
        // updates parent
        if(root->left->parent == NULL)
            root->left->parent = root;
    } else if(value > root->value){
        root->right = insertNode(root->right, value);
        //updates parent
        if(root->right->parent == NULL)
            root->right->parent = root;
    }

    return root;
}

Node* removeNodeNonRecur(Node* root, int value){
    if(root == NULL)
        return root;

    // finds node to be removed and it's parent
    Node* node = findNode(root, value);

    if(node == NULL)
        return root;

    Node* parent = node->parent;

    // Case 1: No children
    if(node->left == NULL && node->right == NULL){
        // if parent exists, update parent
        // else node must be root since it doesnt have a parent, therefore remove root
        if(parent != NULL){
            if(parent->left == node)
                parent->left = NULL;
            else
                parent->right = NULL;
        } else{
            root = NULL;
        }

        free(node);
    }
    // Case 2: 1 child
    else if(node->left == NULL || node->right == NULL){
        Node* child = (node->left) ? node->left : node->right;

        child->parent = node->parent;

        // if parent exists, update parent
        // else node must be root since it doesnt have a parent, therefore replace root
        if(parent != NULL){
            if(parent->left == node)
                parent->left = child;
            else
                parent->right = child;
        } else{
            root = child;
        }

        free(node);
    }
    // Case 3: 2 children
    else{
        // finds the in-order predecessor (max value in left subtree)and it's parent
        Node* predecessor = findMax(node->left);

        // updates parent
        if(predecessor->parent == node)
            node->left = predecessor->left;
        else
            predecessor->parent->right = predecessor->left;

        node->value = predecessor->value;

        free(predecessor);
    }

    return root;
}

Node* removeNode(Node* root, int value){
    if(root == NULL)
        return root;

    // traverse to find the target node
    if(value < root->value)
        root->left = removeNode(root->left, value);
    else if(value > root->value)
        root->right = removeNode(root->right, value);
    // found the target node - handle deletion based on number of children
    else{
        // Case 1: No children
        if(root->left == NULL && root->right == NULL){
            free(root);
            return NULL;
        }
        // Case 2: 1 child
        else if(root->left == NULL || root->right == NULL){
            Node* child = (root->left) ? root->left : root->right;
            child->parent = root->parent;
            free(root);
            return child;
        }
        // Case 3: 2 children
        else{
            // find the in-order predecessor (max value in left subtree)
            Node* predecessor = findMax(root->left);

            // replace current node's value with predecessor's value
            root->value = predecessor->value;

            // remove the predecessor from left subtree
            root->left = removeNode(root->left, predecessor->value);
        }
    }

    return root;
}

Node* rotateLeft(Node* p){
    if(p == NULL || p->right == NULL)
        return p;

    Node* z = p->right;
    Node* T2 = z->left;

    // rotates
    z->left = p;
    p->right = T2;

    // updates nodes' parents
    z->parent = p->parent;
    p->parent = z;
    if(T2 != NULL)
        T2->parent = p;

    return z;
}

Node* rotateRight(Node* p){
    if(p == NULL || p->left == NULL)
        return p;

    Node* z = p->left;
    Node* T2 = z->right;

    // rotates
    z->right = p;
    p->left = T2;

    // updates nodes' parents
    z->parent = p->parent;
    p->parent = z;
    if(T2 != NULL)
        T2->parent = p;

    return z;
}

Node* doubleRotateLeft(Node* root){
    if(root == NULL || root->right == NULL)
        return root;

    root->right = rotateRight(root->right);
    return rotateLeft(root);
}

Node* doubleRotateRight(Node* root){
    if(root == NULL || root->left == NULL)
        return root;

    root->left = rotateLeft(root->left);
    return rotateRight(root);
}

void preOrderPrintNonRecur(Node* root){
    if(root == NULL)
        return;

    // creates and initializes a new stack
    Stack* s = createStack();

    Node* current = root;

    while(current != NULL || !stackIsEmpty(s)){
        // goes through the left path
        while(current != NULL){
            // prints and pushes current node then goes left
            printNode(current);
            push(s, current);
            current = current->left;
        }

        // pops current node we're at
        Node* popped = pop(s);

        // move right
        current = popped->right;
    }

    printf("\n");

    freeStack(s);

    return;
}

void preOrderPrint(Node* root){
    if(root == NULL)
        return;

    printNode(root);
    preOrderPrint(root->left);
    preOrderPrint(root->right);

    return;
}

void inOrderPrintNonRecur(Node* root){
    if(root == NULL)
        return;

    // creates and initializes a new stack
    Stack* s = createStack();

    Node* current = root;

    while(current != NULL || !stackIsEmpty(s)){
        // goes through the left path
        while(current != NULL){
            push(s, current);
            current = current->left;
        }

        // pops and prints current node we're at
        Node* popped = pop(s);

        printNode(popped);

        // after moving back a node by popping it, move right
        current = popped->right;
    }

    printf("\n");

    freeStack(s);

    return;
}

void inOrderPrint(Node* root){
    if(root == NULL)
        return;

    inOrderPrint(root->left);
    printNode(root);
    inOrderPrint(root->right);

    return;
}

void postOrderPrintNonRecur(Node* root){
    if(root == NULL)
        return;

    // creates and initializes 2 new stacks
    Stack* s1 = createStack();
    Stack* s2 = createStack();

    push(s1, root);

    while(!stackIsEmpty(s1)){
        Node* current = pop(s1);
        push(s2, current);
        if(current->left)
            push(s1, current->left);
        if(current->right)
            push(s1, current->right);
    }

    while(!stackIsEmpty(s2)){
        Node* item = pop(s2);
        printNode(item);
    }

    printf("\n");

    freeStack(s1);
    freeStack(s2);

    return;
}

void postOrderPrint(Node* root){
    if(root == NULL)
        return;

    postOrderPrint(root->left);
    postOrderPrint(root->right);
    printNode(root);

    return;
}

void levelOrderPrint(Node* root){
    if(root == NULL)
        return;

    Queue* q = createQueue();

    enqueue(q, root);

    while(!queueIsEmpty(q)){
        Node* current = dequeue(q);
        if(current->left)
            enqueue(q, current->left);
        if(current->right)
            enqueue(q, current->right);

        printNode(current);
    }

    printf("\n");

    free(q);

    return;
}
