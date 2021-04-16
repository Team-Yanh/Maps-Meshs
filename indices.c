#include <stdio.h>
#include <err.h>
#include <stdlib.h>

unsigned int* Array_Indices(int col, int line, unsigned int* indices)
{
    int new_nb_indices = col * line * 2 * 3;
    int count_col = 0;
    unsigned int count_vertex = 0;
    for(int i = 0; i < new_nb_indices; i += 6)
    {
        if(count_col == col)
        {
            count_col = 0;
            count_vertex += 1;
        }
        //First triangle
        indices[i] = count_vertex;
        indices[i + 1] = count_vertex + 1;
        indices[i + 2] = count_vertex + col + 1;

        //Second triangle
        indices[i + 3] = count_vertex + 1;
        indices[i + 4] = count_vertex + col + 1;
        indices[i + 5] = count_vertex + col + 2;
        
        count_vertex += 1;
        count_col += 1;
    }
    return indices;
}
    

