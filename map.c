#include <stdio.h>
#include <stdlib.h>
#include <err.h>

float* Array_map(int col, int line, float* vertices, int nb)
{
    float col_add = 2.0f / col;     //
    float line_add = 2.0f / line;

    int count_col = 0;
    int count_line = 0;
    

    for(int i = 0; i < nb - 2; i+= 3)
    {
        if(count_col == col + 1) //if we are past the end of the line, reset to 0
        {
            count_col = 0;
            count_line += 1;
        }
        
        //handle all 3 coordinates foreach passage
        vertices[i] = count_col * col_add - 1;       //abscissa  
        vertices[i + 1] = count_line * line_add - 1; //ordinate
        vertices[i + 2] = 0.0f;                      //height set to 0

        count_col += 1;
    }
    return vertices;
}


