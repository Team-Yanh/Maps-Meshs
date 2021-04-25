#include <stdio.h>
#include <glad/glad.h>
#include <string.h>
#include "cglm/include/cglm/cglm.h"
#include "opengl.h"
#include "shader_terrain.h"

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aNormal;\n"
    "out vec3 FragPos;\n"
    "out vec3 Normal;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "uniform mat4 model;\n"
    "void main()\n"
    "{\n"
    "   FragPos = vec3(model * vec4(aPos, 1.0));\n"
    "   Normal = aNormal;\n"
    "   gl_Position = projection * view * vec4(FragPos, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 Normal;\n"
    "in vec3 FragPos;\n"
    "uniform vec3 lightPos;\n"
    "uniform vec3 objectColor;\n"
    "uniform vec3 lightColor;\n"
    "void main()\n"
    "{\n"
    "   float ambientStrength = 0.1;\n"
    "   vec3 ambient = ambientStrength * lightColor;\n"
    "   vec3 norm = normalize(Normal);\n"
    "   vec3 lightDir = normalize(lightPos - FragPos);\n"
    "   float diff = max(dot(norm, lightDir), 0.0);\n"
    "   vec3 diffuse = diff * lightColor;\n"
    "   vec3 result = (ambient + diffuse) * objectColor;\n" 
    "   FragColor = vec4(result, 1.0);\n"
    "}\n\0";

const char *cube_vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "uniform mat4 model;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
    "}\0";

const char *cube_fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f);\n"
    "}\n\0";

unsigned int shader_cube()
{
    unsigned int vertex, fragment;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &cube_vertexShaderSource, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &cube_fragmentShaderSource, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertex);
    glAttachShader(shaderProgram, fragment);
    glLinkProgram(shaderProgram);
    checkCompileErrors(shaderProgram, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return shaderProgram;
}


unsigned int shader_terrain()
{
    unsigned int vertex, fragment;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexShaderSource, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertex);
    glAttachShader(shaderProgram, fragment);
    glLinkProgram(shaderProgram);
    checkCompileErrors(shaderProgram, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return shaderProgram;
}


void checkCompileErrors(unsigned int shader, const char* type)
{
    int success;
    char infoLog[1024];
    if(strcmp(type, "PROGRAM") == 0)
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            printf("error during creating the shader");
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            printf("error during creating the program");
        }
    }
}
