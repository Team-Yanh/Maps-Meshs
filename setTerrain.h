#ifndef SETTERRAIN_H
#define SETTERRAIN_H

#include "cglm/include/cglm/cglm.h" 

void set_terrain_normal(float* vertex, int nb_val, int col, int line, float* newHeight);
void set_height(float* height, float* newHeight, int col, int line, int nb_vertex);
void prod_vec(vec3 a, vec3 b, vec3 res);

#endif
