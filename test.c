#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "queue.h"
#include "imageFilter.h"
#include "vector.h"

void print_vec(struct vector *v )
{
    printf("size = %ld \n capacity = %ld \n", v->size,v->capacity);
    for(size_t i = 0; i < v->size;i++)
    {
        printf("%ld -> %p\n",i,*(v->data + i));
    }
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

void testvector()
{

    printf("==== Testing Vector ====\n");
    struct vector *v = vector_new(1);
    int x = 1;
    int *y = &x;
    vector_push(v,y);
    print_vec(v);
    printf("===== pop =====");
    vector_pop(v,(void **)&y);
    print_vec(v);
    vector_free(v);
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
        }
        else if (strcmp(argv[currentArg], "imageFilter") == 0)
        {
            testImageFilter();
        }
        else if (strcmp(argv[currentArg], "testvector") == 0)
        {
            testvector();
        }
        else //default
        {
            printf("Argument not recognize. Current arguments are :\n queue, imageFilter\n");
        }
        currentArg++;
    }

    return 0;
}

