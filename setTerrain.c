#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include "indices.h"
#include "map.h"
#include "opengl.h"
#include <err.h>
#include <stdlib.h>
#include <math.h>
#include "cglm/include/cglm/cglm.h"
#include "shader.h"
#include "shader_terrain.h"
#include <stdbool.h>


void set_terrain_normal(float* vertex, int nb_val, int col, int line, float* newHeight)
{
    float col_add = 2.0f / col;
    float line_add = 2.0f / line;

    int count_col = 0;
    int count_line = 0;

    int cHeight = 0;

    for(int i = 0; i < nb_val; i+= 36)
    {
        int n = i;
        if(count_col == col)
        {
            count_col = 0;
            count_line += 1;
        }
        for(int j = 0; j < 2; j++)
        {
            if(j == 0)
            {
                vertex[n] = count_col * col_add -1;
                vertex[n + 1] = count_line * line_add -1;
                vertex[n + 2] = newHeight[cHeight];

                vertex[n + 6] = (count_col + 1) * col_add - 1;
                vertex[n + 7] = count_line * line_add - 1;
                vertex[n + 8] = newHeight[cHeight + 1];

                vertex[n + 12] = count_col * col_add - 1;
                vertex[n + 13] = (count_line + 1) * line_add - 1;
                vertex[n + 14] = newHeight[cHeight + 2];

                cHeight += 3;
            }
            else
            {
                vertex[n] = (count_col + 1) * col_add - 1;
                vertex[n + 1] = (count_line + 1) * line_add - 1;
                vertex[n + 2] = newHeight[cHeight];

                vertex[n + 6] = count_col * col_add - 1;
                vertex[n + 7] = (count_line + 1) * line_add - 1;
                vertex[n + 8] = newHeight[cHeight + 1];

                vertex[n + 12] = (count_col + 1) * col_add - 1;
                vertex[n + 13] = count_line * line_add - 1;
                vertex[n + 14] = newHeight[cHeight + 2];
            }
            vec3 a;
            a[0] = vertex[n];
            a[1] = vertex[n + 1];
            a[2] = vertex[n + 2];

            vec3 b;
            b[0] = vertex[n + 6];
            b[1] = vertex[n + 7];
            b[2] = vertex[n + 8];

            vec3 c;
            c[0] = vertex[n + 12];
            c[1] = vertex[n + 13];
            c[2] = vertex[n + 14];

            vec3 ab;
            ab[0] = b[0] - a[0];
            ab[1] = b[1] - a[1];
            ab[2] = b[2] - a[2];

            vec3 ac;
            ac[0] = c[0] - a[0];
            ac[1] = c[1] - a[1];
            ac[2] = c[2] - a[2];

            vec3 res;
            prod_vec(ab, ac, res);

            vertex[n + 3] = res[0];
            vertex[n + 4] = res[1];
            vertex[n + 5] = res[2];

            vertex[n + 9] = res[0];
            vertex[n + 10] = res[1];
            vertex[n + 11] = res[2];

            vertex[n + 15] = res[0];
            vertex[n + 16] = res[1];
            vertex[n + 17] = res[2];

            n += 18;
        }
        count_col += 1;
    }
}


void set_height(float* height, float* newHeight, int col, int nb_vertex)
{
    int count_col = 0;
    int count = 0;
    for(int i = 0; i < nb_vertex - (col + 2); i ++)
    {
        if(count_col == col)
        {
            count_col = 0;
            i ++;
        }
        //pour chaque point, créer la hauteur des 2 triangles correspondant.
        newHeight[count]     = height[i];
        newHeight[count + 1] = height[i + 1];
        newHeight[count + 2] = height[i + col + 1];
        
        newHeight[count + 3] = height[i + col + 2];
        newHeight[count + 4] = height[i + col + 1];
        newHeight[count + 5] = height[i + 1];
        count += 6;
        count_col ++;
    } 
}


void prod_vec(vec3 a, vec3 b, vec3 res)
{
    float x = (a[1] * b[2]) - (a[2] * b[1]);
    float y = (a[2] * b[0]) - (a[0] * b[2]);
    float z = (a[0] * b[1]) - (a[1] * b[0]);

    res[0] = x;
    res[1] = y;
    res[2] = z;
}

