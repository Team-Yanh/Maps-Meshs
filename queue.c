#include "queue.h"

Queue *createQueue()
{
    Queue *q = malloc(sizeof(Queue));
    q->front = NULL;
    q->rear = NULL;
    return q;
}

void freeQueue(Queue *q)
{
    while(q->rear != NULL)
        dequeue(q, NULL, NULL);
    free(q);
}
QueueNode *createNode(int x, int y)
{
    QueueNode *qN = malloc(sizeof(QueueNode));
    qN->x = x;
    qN->y = y;
    qN->next = NULL;
    return qN;
}

void enqueue(Queue *q, int x, int y)
{
    QueueNode *qN = createNode(x, y);
    if(q->rear == NULL)
    {
        q->front = qN;
        q->rear = qN;
    }
    else
    {
        q->rear->next = qN;
        q->rear = qN;
    }
}

void dequeue(Queue *q, int *x, int*y)
{
    if(q->front == NULL)
        return;

    QueueNode *qN = q->front;
    q->front = q->front->next;
    *x = qN->x;
    *y = qN->y;

    if(q->front == NULL)
        q->rear = NULL;
    free(qN);
}

int isEmpty(Queue *q)
{
    return q->rear == NULL;
}


void printNode(QueueNode *qN)
{
    printf("Node %p, x : %i, y : %i\n", qN, qN->x, qN->y);
}

void printQueue(Queue *q)
{
    QueueNode *qN = q->front;
    while(qN != NULL)
    {
        printNode(qN);
        qN = qN->next;
    }
}
