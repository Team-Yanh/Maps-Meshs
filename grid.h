#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <err.h>

struct grid
{
    size_t width;
    size_t height;
    int **arr;
};
typedef struct grid grid;

grid *newGrid(size_t width, size_t height);
void freeGrid(grid *g);
void printGrid(grid *g);

