#include <stdlib.h>
#include <stdio.h>
#include <err.h>

#include "grid.h"

int main(void)
{
    grid *g = newGrid(5, 10);
    printGrid(g);
    freeGrid(g);
    
    return 0;
}
