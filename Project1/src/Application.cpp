#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <assert.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Renderer.h"

/* How to use Renderer:
    All you need to give to Renderer is as below:
        VertexArray
        IndexBuffer
        Shader
    Don't need to give VertexBuffer: is't already bound to VertexArray
*/

int main(void)
{
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    // windowHint要在create window之前设置
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (GLEW_OK != glewInit())
        std::cout << "ERROR: failed to initialize GLEW!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    // Add a scope to ensure all GL stuff is destructed before glfwTerminate() which destroy corrent OpenGL context
    {
        float position[] = {
            -0.5f, -0.5f,   //0
             0.5f, -0.5f,   //1
             0.5f,  0.5f,   //2
            -0.5f,  0.5f,   //3
        };
        VertexBuffer VBO(position, sizeof(position));

        VertexBufferLayout layout;
        layout.Push<float>(2);

        VertexArray VAO;
        VAO.AddBuffer(&VBO, &layout);

        unsigned int index[] = {
            0, 1, 2,
            2, 3, 0
        };
        IndexBuffer EBO(index, sizeof(index));

        Shader shader("resources/shaders/Basic.shader");

        Renderer renderer;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            // currently need to set uniform explicitly; will material to include uniform in renderer.
            shader.Bind();
            shader.SetUniform4f("u_Color", 0.0f, 1.0f, 0.0f, 1.0f);
            
            renderer.Draw(&VAO, &EBO, &shader);

            /* Swap front and back buffers */
            CALL_GL_API(glfwSwapBuffers(window));

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}