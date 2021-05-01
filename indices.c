#include <stdio.h>
#include <err.h>
#include <stdlib.h>


unsigned int* Array_Indices(int col, int line, unsigned int* indices)
{
    int count_col = 0;
    int count_line = 0;
    unsigned int nb_vertex = (col + 1) * (line + 1) - (col + 1);
    int i = 0;
    for(unsigned int count_vertex = 0; count_vertex < nb_vertex; count_vertex ++)
    {
        //handle all 3 vertice of the triangle at once
        if(count_col == col && count_line != line - 1)
        {//if we are at the end of a line, "close" the line with a new triangle
            indices[i] = count_vertex;
            indices[i + 1] = count_vertex + col;
            indices[i + 2] = count_vertex + col + 1;
            count_col = 0;
            count_line ++;
            i += 3;
        }
        else
        {//normal calse, create a triangle
            indices[i] = count_vertex;
            indices[i + 1] = count_vertex + 1;
            indices[i + 2] = count_vertex + col + 1;
            count_col ++;
            i += 3;    
                
            if(count_line == line - 1)
            {//draw the last line of triangles with the one before
                indices[i] = count_vertex + 1;       //top right
                indices[i + 1] = count_vertex + col + 1; //bottom left
                indices[i + 2] = count_vertex + col + 2; //bottom right
                i += 3; 
            }
        }

    }
    return indices;
}
    

