#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include "indices.h"
#include "map.h"
#include <err.h>
#include <stdlib.h>
#include <math.h>
#include "cglm/include/cglm/cglm.h"
#include "shader.h"
#include "shader_terrain.h"
#include "imageUtils.h"
#include <SDL/SDL.h>
#include "SDL/SDL_image.h"
#include <stdbool.h>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

vec3 cameraPos = {0.0f, 0.0f, 3.0f};
vec3 cameraFront = {0.0f, 0.0f, -1.0f};
vec3 cameraUp = {0.0f, 1.0f, 0.0f};

bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 800.0f / 2.0f;
float lastY = 600.0f / 2.0f;
float fov = 45.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

vec3 lightPos = {1.2f, 1.0f, 2.0f};

int opengl_Create_Terrain(int col, int line, float* height)
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return -1;
    }
    
    glEnable(GL_DEPTH_TEST);

    unsigned int shaderProgram = shader_terrain();
    unsigned int shaderCube = shader_cube();

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    /*int nb_vertices = (col + 1) * (line + 1) * 3;
    int nb_triangle = col * line * 2;
    int nb_indices = nb_triangle * 3;

<<<<<<< HEAD
    //printf("indices : %d\n", nb_indices);
=======
    //float *vertices = malloc(nb_vertices * sizeof(float));
    //unsigned int *indices = malloc(nb_indices * sizeof(unsigned int));
>>>>>>> nico

    float vertices[nb_vertices];
    Array_map(col, line, vertices, nb_vertices);

    unsigned int indices[nb_indices];
    Array_Indices(col, line, indices);

    vertices[0] = -1.0f;
    vertices[1] = -1.0f;
    vertices[3] = (2.0f / col) - 1;
   
    for(int i = 0; i < nb_vertices - 2; i+= 3)
    {
        printf("Vertices --- i: %d, x: %f, y: %f, z: %f\n", i,  vertices[i], vertices[i + 1], vertices[i + 2]);
    }

    for(int i = 0; i < nb_indices - 2; i += 3)
    {
       printf("Indices --- i: %d, 1st: %u, 2nd: %u, 3rd; %u\n", i, indices[i], indices[i + 1], indices[i + 2]);
    }*/

    /*float vertices_test[] = {
        -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f
    };*/

    int nb_triangle = col * line * 2;
    int nb_vertex = nb_triangle * 3;
    int nb_val = nb_vertex * 6;

    float vertex[nb_val];
    
    float newHeight[nb_vertex];
    /*float height[] = {
        0.5f, 0.5f, 0.5f, 0.5f,
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f
    };*/

    set_height(height, newHeight, col, (col + 1) * (line + 1));
    /*for(int i = 0; i < nb_vertex; i += 3)
    {
        printf("%f, %f, %f\n", newHeight[i], newHeight[i + 1], newHeight[i + 2]);
    }*/
    set_terrain_normal(vertex, nb_val, col, line, newHeight);

    /*for(int i = 0; i < nb_val; i += 6)
    {
        printf("Vertex --- x:%f, y:%f, z:%f\n", vertex[i + 3], vertex[i + 4], vertex[i + 5]);
    }*/

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, 0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, 0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, 0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, 0.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f,  0.0f
    };


    unsigned int VBO, VAO;//, EBO;
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    //glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).

    glBindVertexArray(VAO);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    unsigned int lightCubeVAO, lightCubeVBO;
    glGenVertexArrays(1, &lightCubeVAO);
    glGenBuffers(1, &lightCubeVBO);

    glBindVertexArray(lightCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lightCubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(VAO); 


    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    use(shaderProgram);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        use(shaderProgram);
        vec3 object = {1.0f, 0.5f, 0.31f};
        vec3 light = {1.0f, 1.0f, 1.0f};
        setVec3(shaderProgram, "objectColor", object);
        setVec3(shaderProgram, "lightColor", light);
        setVec3(shaderProgram, "lightPos", lightPos);

        mat4 view;
        mat4 model;
        glm_mat4_identity(model);
        glm_mat4_identity(view);

        vec3 camera2;
        vec3 rot = {3.0f, 0.0f, 0.0f};

        glm_vec3_add(cameraPos, cameraFront, camera2);

        glm_lookat(cameraPos, camera2, cameraUp, view);
        glm_rotate(model, glm_rad(-55.0f), rot);

        setMat4(shaderProgram, "view", view);
        setMat4(shaderProgram, "model", model);

        mat4 projection;
        glm_mat4_identity(projection);
        glm_perspective(glm_rad(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f, projection);
        
        setMat4(shaderProgram, "projection", projection);
        
        //vec4 color = {0.35f, 0.25f, 0.20f, 0.0f};
        //setVec4(shaderProgram, "ourColor", color);

        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, nb_triangle * 3);
        //glDrawElements(GL_TRIANGLES, nb_indices, GL_UNSIGNED_INT, 0);
        
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


   
        use(shaderCube);
        setMat4(shaderCube, "projection", projection);
        setMat4(shaderCube, "view", view);
        glm_mat4_identity(model);
        glm_translate(model, lightPos);
        vec3 scale = {0.2f, 0.2f, 0.2f};
        glm_scale(model, scale);
        setMat4(shaderCube, "model", model);

        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        //glBindVertexArray(0); // no need to unbind it every time 

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 1;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);

    float cameraSpeed = 2.5 * deltaTime;
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        cameraPos[0] += cameraSpeed * cameraFront[0];
        cameraPos[1] += cameraSpeed * cameraFront[1];
        cameraPos[2] += cameraSpeed * cameraFront[2];
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        cameraPos[0] -= cameraSpeed * cameraFront[0];
        cameraPos[1] -= cameraSpeed * cameraFront[1];
        cameraPos[2] -= cameraSpeed * cameraFront[2];
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        vec3 dest;
        glm_vec3_cross(cameraFront, cameraUp, dest);
        glm_vec3_normalize(dest);
        cameraPos[0] -= cameraSpeed * dest[0];
        cameraPos[1] -= cameraSpeed * dest[1];
        cameraPos[2] -= cameraSpeed * dest[2];
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        vec3 dest;
        glm_vec3_cross(cameraFront, cameraUp, dest);
        glm_vec3_normalize(dest);
        cameraPos[0] += cameraSpeed * dest[0];
        cameraPos[1] += cameraSpeed * dest[1];
        cameraPos[2] += cameraSpeed * dest[2];
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------

void framebuffer_size_callback(GLFWwindow* window __attribute__((unused)), int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window __attribute__((unused)), double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensivity = 0.1f;
    xoffset *= sensivity;
    yoffset *= sensivity;

    yaw += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    vec3 front;
    front[0] = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
    front[1] = sin(glm_rad(pitch));
    front[2] = sin(glm_rad(yaw)) * cos(glm_rad(pitch));
    
    glm_vec3_normalize(front);

    cameraFront[0] = front[0];
    cameraFront[1] = front[1];
    cameraFront[2] = front[2];
}

void scroll_callback(GLFWwindow* window __attribute__((unused)), double xoffset __attribute((unused)), double yoffset)
{
    fov -= (float)yoffset;
    if(fov < 1.0f)
        fov = 1.0f;
    if(fov > 45.0f)
        fov = 45.0f;
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
                /*if(test == 2)
                {
                    vertex[n + 2] = 0.5f;
                    test += 1;
                }*/

                vertex[n + 6] = (count_col + 1) * col_add - 1;
                vertex[n + 7] = count_line * line_add - 1;
                vertex[n + 8] = newHeight[cHeight + 1];
                /*if(test < 3)
                {
                    vertex[n + 8] = 0.5f;
                    test += 1;
                }*/

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
                /*if(test < 3)
                {
                    vertex[n + 14] = 0.5f;
                    test += 1;
                }*/
                cHeight += 3;
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
            //printf("%f, %f, %f\n", res[0], res[1], res[2]);

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


void display_terrain()
{
    SDL_Surface *image;

    image = IMG_Load("images/out2.png");

    int nbRows = image->h / 10;
    int nbCols = image->w / 10;
    float *heightlist = heightList(image, nbRows, nbCols);

    int col = nbCols - 1;
    int line = nbRows - 1;

    opengl_Create_Terrain(col, line, heightlist);

    free(heightlist);
}
