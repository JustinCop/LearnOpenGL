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


struct ShaderSource
{
    std::string vsSource;
    std::string fsSource;
};

static ShaderSource ParseShader(const std::string &filePath)
{
    enum class ShaderType
    {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    std::ifstream stream(filePath);

    std::string line;

    std::stringstream ss[2];

    ShaderType type = ShaderType::NONE;

    while (getline(stream, line))
    {
        if (line.find("#shader") != line.npos)
        {
            if (line.find("vertex") != line.npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != line.npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            ss[(int)type] << line << "\n";
        }
    }

    ShaderSource source;
    source.vsSource = ss[(int)ShaderType::VERTEX].str();
    source.fsSource = ss[(int)ShaderType::FRAGMENT].str();

    return source;
}

static unsigned int CompileShader(GLenum type, const std::string &source)
{
    unsigned int shader = glCreateShader(type);
    const char *src = source.c_str();
    CALL_GL_API(glShaderSource(shader, 1, &src, nullptr));
    CALL_GL_API(glCompileShader(shader));

    // Error checking
    int result;
    CALL_GL_API(glGetShaderiv(shader, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        int length;
        CALL_GL_API(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length));
        char *message = (char*)alloca(length * sizeof(char));   // alloca直接在栈上分配
        CALL_GL_API(glGetShaderInfoLog(shader, length, &length, message));
        std::cout << "Failed to compile : " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
        std::cout << message << std::endl;
        CALL_GL_API(glDeleteShader(shader)) ;
        return 0;
    }

    return shader;
}

static unsigned int CreateShader(const std::string& vsSource, const std::string& fsSource)
{
    unsigned int program = glCreateProgram();

    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vsSource);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fsSource);
    
    CALL_GL_API(glAttachShader(program, vs));
    CALL_GL_API(glAttachShader(program, fs));

    CALL_GL_API(glLinkProgram(program));
    CALL_GL_API(glValidateProgram(program));

    CALL_GL_API(glDetachShader(program, vs));
    CALL_GL_API(glDetachShader(program, fs));

    CALL_GL_API(glDeleteShader(vs));
    CALL_GL_API(glDeleteShader(fs));

    return program;
}


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
        layout.Push(2, GL_FLOAT);

        VertexArray VAO;
        VAO.AddBuffer(VBO, layout);

        unsigned int index[] = {
            0, 1, 2,
            2, 3, 0
        };
        IndexBuffer EBO(index, sizeof(index));


        ShaderSource source = ParseShader("resources/shaders/Basic.shader");
        unsigned int shader = CreateShader(source.vsSource, source.fsSource);

        int location = glGetUniformLocation(shader, "u_Color");
        assert(location >= 0);

        float r = 0.00f;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            CALL_GL_API(glClear(GL_COLOR_BUFFER_BIT));

            CALL_GL_API(glUseProgram(shader));
            CALL_GL_API(glUniform4f(location, r, 1.0f, 0.0f, 1.0f));

            VAO.Bind();
            VBO.Bind();
            EBO.Bind();
            CALL_GL_API(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)0));

            if (r >= 1.0f)
                r = 0.0f;
            r += 0.01f;

            /* Swap front and back buffers */
            CALL_GL_API(glfwSwapBuffers(window));

            /* Poll for and process events */
            glfwPollEvents();
        }

        CALL_GL_API(glDeleteProgram(shader));
    }
    glfwTerminate();
    return 0;
}