#ifndef SHADER_TERRAIN_H
#define SHADER_TERRAIN_H

#include <stdio.h>
#include <glad/glad.h>
#include "opengl.h"
//#include "glm/include/cglm/cglm.h"

int shader_terrain();
void checkCompileErrors(unsigned int shader, const char* type);

#endif
