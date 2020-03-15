#pragma once
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <map>

struct ShaderSource
{
    std::string vsSource;
    std::string fsSource;
};


class Shader
{
private:
    const std::string m_FilePath;
    unsigned int m_RendererID;
    std::map<const char*, int> m_UniformLocations;

    ShaderSource ParseShader(const std::string &filePath);
    unsigned int CompileShader(GLenum type, const std::string &source);
    unsigned int CreateShader(const std::string &vsSource, const std::string &fsSource);

public:
    Shader(const std::string &filePath);
    ~Shader();
    void Bind();
    void UnBind();

    bool SetUniform4f(const char *name, float v0, float v1, float v2, float v3);
};