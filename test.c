#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "queue.h"
#include "filters.h"
#include "imageFilter.h"

//void printNode(QueueNode *qN);
//void printQueue(Queue *q);

void testQueue()
{
    printf("==== Testing Queue ====\n");
    Queue *q = createQueue();
    printQueue(q);
    printf("isEmpty ? %d Expected True 1 \n", isEmpty(q));

    enqueue(q, 1, 1);
    enqueue(q, 2, 2);
    enqueue(q, 3, 3);
    printQueue(q);
    printf("isEmpty ? %d Expected False 0 \n", isEmpty(q));

    int x = 0;
    int y = 0;
    dequeue(q, &x, &y);
    printf("x : %d, y: %d, Expected 1,1\n", x, y);
    dequeue(q, &x, &y);
    dequeue(q, &x, &y);
    printf("x : %d, y: %d Expected 3,3\n", x, y);

    printf("Queue is empty, trying to dequeue again\n");
    dequeue(q, &x, &y);
    printf("x : %d, y: %d Expected 3,3\n", x, y);

    printf("isEmpty ? %d Expected True 1 \n", isEmpty(q));
    printf("==== Queue Testing Done ====\n");
}

void testFilters()
{
    printf("==== Testing Filters ====\n");
    printf("==== Filters Testing Done ====\n");
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
        printf("No file to test, current arguments are :\n
                    queue, filters, imageFilter\n");
        return 0;
    }
    while(currentArg < argc)
    {
        if(strcmp(argv[currentArg], "queue") == 0)
        {
            testQueue();
        }
        else if (strcmp(argv[currentArg], "filters") == 0)
        {
            testFilters();
        }
        else if (strcmp(argv[currentArg], "imageFilter") == 0)
        {
            testImageFilter();
        }
        else //default
        {
            printf("Argument not recognize. Current arguments are :\n
                    queue, filters, imageFilter\n");
        }
        currentArg++;
    }

    return 0;
}

