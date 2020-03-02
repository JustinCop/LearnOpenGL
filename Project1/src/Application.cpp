#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>


static unsigned int CompileShader(GLenum type, const std::string &source)
{
    unsigned int shader = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    // Error checking
    int result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        char *message = (char*)alloca(length * sizeof(char));   // alloca直接在栈上分配
        glGetShaderInfoLog(shader, length, &length, message);
        std::cout << "Failed to compile : " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

static unsigned int CreateShader(const std::string& vsSource, const std::string& fsSource)
{
    unsigned int program = glCreateProgram();

    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vsSource);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fsSource);
    
    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);
    glValidateProgram(program);

    glDetachShader(program, vs);
    glDetachShader(program, fs);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}


int main(void)
{
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (GLEW_OK != glewInit())
        std::cout << "ERROR: failed to initialize GLEW!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    float position[] = {
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f
    };

    unsigned int a;
    glGenBuffers(1, &a);
    glBindBuffer(GL_ARRAY_BUFFER, a);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), position, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);   // 最后一个参数也许是为了节省带宽，所以把offset转成（void*）因为它只有1 byte

    const std::string vertexShader = 
        "#version 330 core\n"
        "layout(location = 0) in vec4 position;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = position;\n"
        "}\n";

    const std::string fragmentShader = 
        "#version 330 core\n"
        "layout(location = 0) out vec4 color;\n"
        "void main()\n"
        "{\n"
        "    color = vec4(1.0, 0.0, 0.0, 1.0);\n"
        "}\n"
        "";

    unsigned int shader = CreateShader(vertexShader, fragmentShader);

    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}