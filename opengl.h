#ifndef OPENGL_H
#define OPENGL_H

#include <GLFW/glfw3.h>
#include "cglm/include/cglm/cglm.h"

int opengl_Create_Terrain(int col, int line, float* heightTerrain);
void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void prod_vec(vec3 a, vec3 b, vec3 res);
void set_height(float* height, float* newHeight, int col, int nb_vertex);
void set_terrain_normal(float* vertex, int nb_vertex, int col, int line, float* newHeight);
void display_terrain(char* filename);

#endif
