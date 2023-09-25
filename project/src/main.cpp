#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main(void)
{
    /* Initialize the library */
    if (!glfwInit()) 
    {
        std::cout << "GLFW initialization failed" << std::endl;
        return -1;
    }

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window;
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        std::cout << "Failed to create a window" << std::endl;
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* GLAD loads all OpenGL function pointers */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }    

    std::cout << glGetString(GL_VERSION) << std::endl;

    /* Coordinates for two triangles combining into a square */
    GLfloat vertices[] = {
        -0.5f, -0.5f, // Bottom left 0
         0.5f, -0.5f, // Bottom right 1
         0.5f,  0.5f, // Top right 2
        -0.5f,  0.5f, // Top left 3 
    };
    
    GLuint indices[] = {
        0, 1, 3, // Left triangle
        3, 1, 2 // Right triangle
    };

    /* Generate and bind VAO */
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    /* Generate and bind VBO then load vertex data into it */
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    /* Generate and bind EBO then load index data into it */
    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    /* Specify how VAO should interpret the VBO */
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(vertices[0]), (void*)0);
    glEnableVertexAttribArray(0);

    /* -------------------------Shaders---------------------------- */
    // VERTEX
    const char* vertexShaderSource = R"(#version 330 core
    layout (location = 0) in vec2 aPos;
    void main()
    {
        gl_Position = vec4(aPos, 0.0, 1.0);
    }
    )";
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // FRAGMENT
    const char* fragmentShaderSource = R"(#version 330 core
    out vec4 FragColor;
    void main()
    {
        FragColor = vec4(0.5f, 0.1f, 0.8f, 1.0f);
    }
    )";
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Link shaders into a program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Delete shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    /* ------------------------------------------------------------ */

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // Clean up back buffer
        glClear(GL_COLOR_BUFFER_BIT);
        // Use shader program for rendering
        glUseProgram(shaderProgram);
        // Draw call to draw a triangle
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }

    // Delete objects
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
    // Delete window
    glfwDestroyWindow(window);
    // Terminate GLFW
    glfwTerminate();
    return 0;
}