#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <err.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\0";
int main()
{
    ///////////////////
    // #[Variables]# //
    ///////////////////

    // - Triangle vertices array
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, //Bottom left
        -0.5f, 0.5f, 0.0f,  //Top left
        0.5f, 0.5f, 0.0f,   //Top right
        0.5f, -0.5f, 0.0f   //Bottom right
    };

    unsigned int indices[] = {
        0, 1, 3,
        3, 2, 1
    };

    // - Buffers
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    // - Error managing
    int success;
    char infoLog[512];

    //////////////////////////////////////////////////////
    // #[ Creating the context and initializing glfw ]# //
    //////////////////////////////////////////////////////

    // - Init glfw and its context
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // - Init the window
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);

    if (window == NULL)
    {
        glfwTerminate();
        errx(-1, "Failed to create GLFW window");
    }

    glfwMakeContextCurrent(window);

    // - Call back to keep window dimensions accurate with user resizing
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // - Init glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        errx(-1, "Failed to initialize GLAD");

    ///////////////////////////////////////////////////////
    // #[ Creating and compiling shaders then linking ]# //
    ///////////////////////////////////////////////////////

    // - Creating vertexShader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // - Checking error while compiling the vertex shader
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("Compilation vertex shader failed:\n%s", infoLog);
    }

    // - Creating fragment shader
        unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // - Checking error while compiling the fragment shader
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("Compilation fragment shader failed:\n%s", infoLog);
    }

    // - Creating the shader program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // - Checking error while linking the shaders to the program
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("Linking shader failed:\n%s", infoLog);
    }

    // - Deleting the compiled shader that we do not need anymore
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    ////////////////////////////////////////////////
    // #[ Binding buffer to our vertices array ]# //
    ////////////////////////////////////////////////

    // - Generate arrays object and vertex buffer
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // - Bind vertex array object then set vertex buffer
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // - Linking Vertex Attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // - Unbinding
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    ///////////////////////
    // #[ Render Loop ]# //
    ///////////////////////

    // - Setting clear color
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // - Render loop
    while(glfwWindowShouldClose(window) == 0)
    {
        // - Check escape key to quit
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, 1);

        // - Clear
        glClear(GL_COLOR_BUFFER_BIT);

        // - Draw triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // - Swap buffers and get inputs
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //////////////////////
    // #[Unallocating]# //
    //////////////////////

    // Clear vertex array objects and vertex buffer
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // Clearing all previously allocated GLFW resources
    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    if (window)
        glViewport(0, 0, width, height);
}
