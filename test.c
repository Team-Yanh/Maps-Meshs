#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "queue.h"
#include "imageFilter.h"

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

void testQueueFree()
{
    printf("testQueueFree : init\n");
    Queue *q = createQueue();
    printf("testQueueFree : enqueue 1\n");
    enqueue(q, 1, 1);
    printf("testQueueFree : free\n");
    freeQueue(q);
}

void testQueue()
{
    printf("==== Testing Queue ====\n");
    Queue *q = createQueue();
    printQueue(q);
    printf("isEmpty ? %d Expected True 1 \n", isEmpty(q));

    printf("Enqueuing 1, 2, 3, 4, 5\n");
    enqueue(q, 1, 1);
    enqueue(q, 2, 2);
    enqueue(q, 3, 3);
    enqueue(q, 4, 4);
    enqueue(q, 5, 5);
    printQueue(q);
    printf("isEmpty ? %d Expected False 0 \n", isEmpty(q));

    int x = 0;
    int y = 0;
    printf("Dequeuing\n");
    dequeue(q, &x, &y);
    printf("x : %d, y: %d, Expected 1,1\n", x, y);
    printf("Dequeuing\n");
    dequeue(q, &x, &y);
    printf("x : %d, y: %d, Expected 2,2\n", x, y);
    printf("Dequeuing\n");
    dequeue(q, &x, &y);
    printf("x : %d, y: %d Expected 3,3\n", x, y);

    printf("Dequeuing 4\n");
    dequeue(q, &x, &y);
    printf("Dequeuing 5\n");
    dequeue(q, &x, &y);
    printf("x, y : %d %d\n", x, y);
    printf("Queue is empty, trying to dequeue again\n");
    dequeue(q, &x, &y);
    printf("x, y : %d %d Values shouldnt change\n", x, y);

    printf("isEmpty ? %d Expected True 1 \n", isEmpty(q));
    printf("==== Queue Testing Done ====\n");
    freeQueue(q);
}

void testImageFilter()
{
    printf("==== Testing imageFilter ====\n");
    printf("==== imageFilter Testing Done ====\n");
}


int main(int argc, char *argv[])
{
    int currentArg = 1;
    if(argc == 1)
    {
        printf("No file to test, current arguments are :\n queue, imageFilter\n");
        return 0;
    }
    while(currentArg < argc)
    {
        if(strcmp(argv[currentArg], "queue") == 0)
        {
            testQueue();
            testQueueFree();
        }
        else if (strcmp(argv[currentArg], "imageFilter") == 0)
        {
            testImageFilter();
        }
        else //default
        {
            printf("Argument not recognize. Current arguments are :\n queue, imageFilter\n");
        }
        currentArg++;
    }

    return 0;
}

