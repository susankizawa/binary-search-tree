#include<stdio.h>
#include<stdlib.h>

// Todo: done :^^D

// Node definition ok
// createNode ok
// printNode ok
// insertNode ok (nonrecur)
// removeNode ok (nonrecur)
// preOrderPrint ok
// inOrderPrint ok
// postOrderPrint ok
// levelOrderPrint ok
// findMin ok
// findMax ok
// findNode ok (nonrecur)
// getHeight ok

// Stack definition ok
// initializeStack ok - replaced with createStack ok
// pop ok
// push ok
// stackIsFull ok
// stackIsEmpty ok
// printStack ok
// freeStack added ok

// Queue definition ok
// initializeQueue replaced with createQueue ok
// enqueue ok
// dequeue ok
// queueIsEmpty ok
// queueIsFull added ok
// printQueue ok
// freeQueue added ok
// obs.: turned the queue into a circular queue due to not being able to enqueue new nodes once the end of the queue reached the end of the array

#define MAX_SIZE 10

// Data structs

typedef struct Node{
    int value;
    struct Node* left;
    struct Node* right;
} Node;

typedef struct Stack{
    Node* items[MAX_SIZE];
    int top;
} Stack;

typedef struct Queue{
    Node* items[MAX_SIZE];
    int start;
    int end;
    int size;
} Queue;

// Node

Node* createNode(int value){
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->value = value;
    newNode->left = newNode->right = NULL;
    return newNode;
}

void printNode(Node* n){
    if(n == NULL){
        printf("[ NULL ]");
        return;
    }

    printf("[ %d ]", n->value);
    return;
}

// Stack

int stackIsFull(Stack* s){
    if(s->top == (MAX_SIZE - 1))
        return 1;
    else
        return 0;
}

int stackIsEmpty(Stack* s){
    if(s->top == -1)
        return 1;
    else
        return 0;
}

Stack* createStack(){
    Stack* newStack = (Stack*)malloc(sizeof(Stack));
    newStack->top = -1;
    return newStack;
}

void freeStack(Stack* s){
    if(!stackIsEmpty(s)){
        printf("Error: cannot free stack - stack is not empty!\n");
        return;
    }

    free(s);
    printf("Stack successfully freed!\n");

    return;
}

void push(Stack* s, Node* item){
    if(stackIsFull(s)){
        printf("Error: stack is full!\n");
        return;
    }

    // increments top by one then adds an item to the top position;
    s->items[++s->top] = item;
    return;
}

Node* pop(Stack* s){
    if(stackIsEmpty(s)){
        printf("Error: stack is empty!\n");
        return NULL;
    }

    // returns the item on the top position, then decrements top by one;
    return s->items[s->top--];
}

void printStack(Stack* s){
    if(stackIsEmpty(s)){
        printf("Stack is empty.\n");
        return;
    }

    printf("Stack:\n");
    for(int i = s->top;i > -1;i--){
        printNode(s->items[i]);
        printf("\n");
    }
    return;
}

// Queue

int queueIsEmpty(Queue* q){
    return (q->size == 0);
}

int queueIsFull(Queue* q){
    return (q->size == MAX_SIZE);
}

Queue* createQueue(){
    Queue* newQueue = (Queue*)malloc(sizeof(Queue));
    newQueue->start = 0;
    newQueue->end = -1;
    newQueue->size = 0;
    return newQueue;
}

void enqueue(Queue* q, Node* node){
    if(queueIsFull(q)){
        printf("Error: cannot enqueue - queue is full!\n");
        return;
    }

    q->end = (q->end + 1) % MAX_SIZE;
    q->items[q->end] = node;
    q->size++;

    return;
}

Node* dequeue(Queue* q){
    if(queueIsEmpty(q)){
        printf("Error: cannot dequeue - queue is empty!\n");
        return NULL;
    }

    Node* node = q->items[q->start];
    q->start = (q->start + 1) % MAX_SIZE;
    q->size--;

    return node;
}

void printQueue(Queue* q){
    if(queueIsEmpty(q)){
        printf("Queue is empty.\n");
        return;
    }

    printf("Queue:\n");

    int p = q->start;
    for(int i = 0; i < q->size; i++){
        printNode(q->items[i]);
        p = (p + 1) % MAX_SIZE;
    }
    printf("\n");
    return;
}

void freeQueue(Queue* q){
    if(!queueIsEmpty(q)){
        printf("Error: cannot free queue - queue is not empty!\n");
        return;
    }

    free(q);
    printf("Queue successfully freed!\n");

    return;
}

// Binary Search Tree

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
    Node* node = root;
    Node* parent = NULL;

    while(node != NULL && node->value != value){
        parent = node;
        if (value < node->value)
            node = node->left;
        else if(value > node->value)
            node = node->right;
    }

    if(node == NULL)
        return root;

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
    }
    // Case 2: 1 child
    else if(node->left == NULL || node->right == NULL){
        Node* child = (node->left) ? node->left : node->right;

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
    }
    // Case 3: 2 children
    else{
        // finds node of maximum value of left subtree and it's parent
        Node* successor = node->left;
        Node* successorParent = NULL;

        while(successor->right != NULL){
            successorParent = successor;
            successor = successor->right;
        }

        // updates successor's parent if it exists
        if(successorParent != NULL){
            successorParent->right = successor->left;
        } else{
            node->left = successor->left;
        }

        node->value = successor->value;

        free(successor);
    }

    free(node);

    return root;
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

int getHeight(Node* root){
    if(root == NULL)
        return 0;

    Queue* q = createQueue();

    enqueue(q, root);
    int height = 0;

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


int main(){
    Node* root = createNode(50);

    printf("height: %d\n", getHeight(root));

    insertNodeNonRecur(root, 10);
    insertNodeNonRecur(root, 20);
    insertNodeNonRecur(root, 90);
    insertNodeNonRecur(root, 80);
    insertNodeNonRecur(root, 30);
    insertNodeNonRecur(root, 60);
    insertNodeNonRecur(root, 40);
    insertNodeNonRecur(root, 70);

    levelOrderPrint(root);

    printf("height: %d\n", getHeight(root));

    return 0;
}

