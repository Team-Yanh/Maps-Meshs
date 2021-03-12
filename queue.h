#pragma once

#include <stdlib.h>
#include <stdio.h>

typedef struct QueueNode
{
    int x;
    int y;
    struct QueueNode *next;
}QueueNode;

typedef struct Queue
{
    QueueNode *front;
    QueueNode *rear;
}Queue;

Queue *createQueue();
void freeQueue(Queue *q);
QueueNode *createNode(int x, int y);
void enqueue(Queue *q, int x, int y);
void dequeue(Queue *q, int *x, int*y);
int isEmpty(Queue *q);

void printNode(QueueNode *qN);
void printQueue(Queue *q);
