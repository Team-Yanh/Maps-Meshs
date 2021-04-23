#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <stdio.h>
#include <stdbool.h>


void use(unsigned int shader)
{
    glUseProgram(shader);
}
void setBool(unsigned int shader, const char* name, bool value)
{
    glUniform1i(glGetUniformLocation(shader, name), (int)value);
}

void setInt(unsigned int shader, const char* name, int value)
{
    glUniform1i(glGetUniformLocation(shader, name), value);
}

void setFloat(unsigned int shader, const char* name, float value)
{
    glUniform1i(glGetUniformLocation(shader, name), value);
}

void setMat4(unsigned int shader, const char* name, mat4 mat)
{
    glUniformMatrix4fv(glGetUniformLocation(shader, name), 1, GL_FALSE, &mat[0][0]);
}

void setVec4(unsigned int shader, const char* name, vec4 vec)
{
    glUniform4f(glGetUniformLocation(shader, name), vec[0], vec[1], vec[2], vec[3]);
}

void setVec3(unsigned int shader, const char* name, vec3 vec)
{
    glUniform3f(glGetUniformLocation(shader, name), vec[0], vec[1], vec[2]);
}

#endif

