#ifndef STACK_H
#define STACK_H

#include "config.h"

typedef struct Node Node;

typedef struct Stack{
    Node* items[MAX_SIZE];
    int top;
} Stack;

int stackIsFull(Stack* s);

int stackIsEmpty(Stack* s);

Stack* createStack();

void freeStack(Stack* s);

void printStack(Stack* s);

void push(Stack* s, Node* item);

Node* pop(Stack* s);

#endif // STACK_H
