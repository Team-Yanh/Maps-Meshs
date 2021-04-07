#ifndef OPENGL_H
#define OPENGL_H

#include <GLFW/glfw3.h>

int opengl_Create_Terrain(int col, int line);
void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

#endif
