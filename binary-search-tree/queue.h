#ifndef QUEUE_H
#define QUEUE_H

#include "config.h"

typedef struct Node Node;

typedef struct Queue{
    Node* items[MAX_SIZE];
    int start;
    int end;
    int size;
} Queue;

int queueIsEmpty(Queue* q);

int queueIsFull(Queue* q);

Queue* createQueue();

void freeQueue(Queue* q);

void printQueue(Queue* q);

void enqueue(Queue* q, Node* node);

Node* dequeue(Queue* q);

#endif // QUEUE_H
