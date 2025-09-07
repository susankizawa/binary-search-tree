#include "bst_non_recur.h"

#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "queue.h"

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

int getHeight(Node* root){
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

Node* findNodeNonRecur(Node* root, int value){
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

Node* removeNodeNonRecur(Node* root, int value){
    if(root == NULL)
        return root;

    // finds node to be removed and it's parent
    Node* node = findNodeNonRecur(root, value);

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
        // finds node of maximum value of left subtree (the in-order predecessor) and it's parent
        Node* successor = findMax(node->left);
        Node* successorParent = successor->parent;

        // updates successor's parent if it exists
        if(successorParent != NULL){
            successorParent->right = successor->left;
        } else{
            node->left = successor->left;
        }

        // updates left child's parent if it exists
        if(successor->left != NULL){
            successor->left->parent = successorParent;
        }

        node->value = successor->value;

        free(successor);
    }

    free(node);

    return root;
}

Node* rotateLeft(Node* p){
    Node* parent = p->parent;
    Node* z = p->right;
    Node* T2 = z->left;

    // rotates
    z->left = p;
    p->right = T2;

    // updates childs' parents
    p->parent = z;
    if(T2 != NULL)
        T2->parent = p;

    // updates parent
    if(parent){
        if(parent->right == p)
            parent->right = z;
        else if(parent->left == p)
            parent->left = z;
    }

    return z;
}

Node* rotateRight(Node* p){
    Node* parent = p->parent;
    Node* z = p->left;
    Node* T2 = z->right;

    // rotates
    z->right = p;
    p->left = T2;

    // updates childs' parents
    p->parent = z;
    if(T2 != NULL)
        T2->parent = p;

    // updates parent
    if(parent){
        if(parent->right == p)
            parent->right = z;
        else if(parent->left == p)
            parent->left = z;
    }

    return z;
}

Node* doubleRotateLeft(Node* root){
    rotateRight(root->right);
    return rotateLeft(root);
}

Node* doubleRotateRight(Node* root){
    rotateLeft(root->left);
    return rotateRight(root);
}

void preOrderPrint(Node* root){
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

void inOrderPrint(Node* root){
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

void postOrderPrint(Node* root){
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
