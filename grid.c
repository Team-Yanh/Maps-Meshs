#include "grid.h"

grid *newGrid(size_t width, size_t height)
{
    grid *g = malloc(sizeof(grid));
    if(g == NULL)
        errx(1, "newGrid: error on allocation\n");
    g->width = width;
    g->height = height;
    g->arr = calloc(height, sizeof(int*));
    if(g == NULL)
        errx(1, "newGrid: error on allocation\n");
    for(size_t i = 0; i < height; i++)
    {
        g->arr[i] = calloc(width, sizeof(int));
        if(g->arr[i] == NULL)
            errx(1, "newGrid: error on allocation\n");
    }
    return g;
}

void freeGrid(grid *g)
{
    for(size_t i = 0; i < g->height; i++)
    {
        free(g->arr[i]);
    }
    free(g->arr);
    free(g);
}

void printGrid(grid *g)
{
    for (size_t i = 0; i < g->height; i++)
    {
        for(size_t j = 0; j < g->width; j++)
        {
            printf("%d ", g->arr[i][j]);
        }
        printf("\n");
    }
}

