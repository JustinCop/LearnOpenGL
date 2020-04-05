#include "Shader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Renderer.h"

ShaderSource Shader::ParseShader(const std::string &filePath)
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

unsigned int Shader::CompileShader(GLenum type, const std::string &source)
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
        char *message = (char *)alloca(length * sizeof(char));   // alloca直接在栈上分配
        CALL_GL_API(glGetShaderInfoLog(shader, length, &length, message));
        std::cout << "Failed to compile : " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
        std::cout << message << std::endl;
        CALL_GL_API(glDeleteShader(shader));
        return 0;
    }
    return shader;
}

unsigned int Shader::CreateShader(const std::string &vsSource, const std::string &fsSource)
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

int Shader::GetUniformLocation(const char *name)
{
    // query if locations is already cached in this shader object, if not, find the actual location and cache it.
    if (m_UniformLocations.find(name) != m_UniformLocations.end())
        return m_UniformLocations[name];

    int location = CALL_GL_API(glGetUniformLocation(m_RendererID, name));
    if (location == -1)
    {
        std::cout << "ERROR: shader doesn't have uniform variable: " << name << std::endl;
    }
    
    m_UniformLocations[name] = location;
    return location;
}

Shader::Shader(const std::string &filePath):
    m_FilePath(filePath),
    m_RendererID(0)
{
    ShaderSource source = ParseShader(filePath);
    m_RendererID = CreateShader(source.vsSource, source.fsSource);
}

Shader::~Shader()
{
    CALL_GL_API(glDeleteProgram(m_RendererID));
}

void Shader::Bind()
{
    CALL_GL_API(glUseProgram(m_RendererID));
}

void Shader::UnBind()
{
    CALL_GL_API(glUseProgram(0));
}

void Shader::SetUniform1i(const char *name, int value)
{
    CALL_GL_API(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const char *name, float v0, float v1, float v2, float v3)
{
    CALL_GL_API(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}
