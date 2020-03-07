#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <assert.h>


void ValidateOglApiCall(const char *apiCallStr, const char* file, unsigned int line)
{
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cout << "Error: 0x" << std::hex << error << " encountered when calling: " << apiCallStr << " at file: " << file << " , line " << std::dec << line << std::endl;
        __debugbreak();
    }
}

#define CALL_GL_API(apiCall) apiCall;\
    ValidateOglApiCall(#apiCall, __FILE__, __LINE__);


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

    float position[] = {
        -0.5f, -0.5f,   //0
         0.5f, -0.5f,   //1
         0.5f,  0.5f,   //2
        -0.5f,  0.5f,   //3
    };

    unsigned int VAO;
    CALL_GL_API(glGenVertexArrays(1, &VAO));
    CALL_GL_API(glBindVertexArray(VAO));

    unsigned int VBO;
    CALL_GL_API(glGenBuffers(1, &VBO));
    CALL_GL_API(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    CALL_GL_API(glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW));

    CALL_GL_API(glEnableVertexAttribArray(0));
    CALL_GL_API(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0));   // 最后一个参数也许是为了节省带宽，所以把offset转成（void*）因为它只有1 byte
    CALL_GL_API(glBindBuffer(GL_ARRAY_BUFFER, 0));

    CALL_GL_API(glBindVertexArray(0));


    unsigned int index[] = {
        0, 1, 2,
        2, 3, 0
    };

    unsigned int EBO;
    CALL_GL_API(glGenBuffers(1, &EBO));
    CALL_GL_API(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
    CALL_GL_API(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW));
    CALL_GL_API(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

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

        CALL_GL_API(glBindVertexArray(VAO));
        CALL_GL_API(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
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

    glfwTerminate();
    return 0;
}