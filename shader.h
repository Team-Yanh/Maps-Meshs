#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <stdio.h>
#include <stdbool.h>

typedef struct Shader Shader;
struct Shader
{
    unsigned int ID;
};


    int new_Shader(const char* vertexPath, const char* fragmentPath)
    {
        unsigned int ID;
        char* vertexCode;
        char* fragmentCode;
        const char* vShaderCode;
        const char* fShaderCode;
        FILE* vShaderFile;
        FILE* fShaderFile;
        long fnbBytes;
        long vnbBytes;

        //try ?
        vShaderFile = fopen(vertexPath, "r");
        if(vShaderFile == NULL)
            errx(EXIT_FAILURE, "no file for vertexPath");
        fShaderFile = fopen(fragmentPath, "r");
        if(fShaderFile == NULL)
            errx(EXIT_FAILURE, "no file for fragmentPath");
    
        //Read vShaderFile
        fseek(vShaderFile, 0L, SEEK_END);
        vnbBytes = ftell(vShaderFile);
        fseek(vShaderFile, 0L, SEEK_SET);
        vertexCode = (char*)calloc(vnbBytes, sizeof(char));
        vShaderCode = (char*)calloc(vnbBytes, sizeof(char));
        if(vertexCode == NULL)
            errx(EXIT_FAILURE, "no Buffer, vertex");
        fread(vertexCode, sizeof(char), vnbBytes, vShaderFile);
        fclose(vShaderFile);

        //Read fShaderFile
        fseek(fShaderFile, 0L, SEEK_END);
        fnbBytes = ftell(fShaderFile);
        fseek(fShaderFile, 0L, SEEK_SET);
        fragmentCode = (char*)calloc(fnbBytes, sizeof(char));
        fShaderCode = (char*)calloc(fnbBytes, sizeof(char));
        if(fragmentCode == NULL)
            errx(EXIT_FAILURE, "no Buffer, fragment");
        fread(fragmentCode, sizeof(char), fnbBytes, fShaderFile);
        fclose(fShaderFile);
        
        unsigned int vertex, fragment;
        int success;
        char infoLog[512];

        vShaderCode = (char*)vertexCode;
        fShaderCode = (char*)fragmentCode;
        
        printf("%s\n", vShaderCode);


        //Compile vertexShader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, vnbBytes, &vShaderCode, NULL);
        glCompileShader(vertex);
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            printf("%s", infoLog);
        }

        //Compile fragmentShader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, fnbBytes, &fShaderCode, NULL);
        glCompileShader(fragment);
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            printf("%s", infoLog);
        }

        //Shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            printf("%s", infoLog);
        }

        glDeleteShader(vertex);
        glDeleteShader(fragment);

        return ID;
    }

    void use(Shader shader)
    {
        glUseProgram(shader.ID);
    }
    void setBool(Shader shader, const char* name, bool value)
    {
        glUniform1i(glGetUniformLocation(shader.ID, name), (int)value);
    }

    void setInt(Shader shader, const char* name, int value)
    {
        glUniform1i(glGetUniformLocation(shader.ID, name), value);
    }

    void setFloat(Shader shader, const char* name, float value)
    {
        glUniform1i(glGetUniformLocation(shader.ID, name), value);
    }

#endif

