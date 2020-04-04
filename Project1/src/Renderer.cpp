#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include <iostream>

void ValidateOglApiCall(const char *apiCallStr, const char *file, unsigned int line)
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

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::Clear()
{
    CALL_GL_API(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(VertexArray *pVao, IndexBuffer *pEbo, Shader *pShader)
{
    pVao->Bind();
    pEbo->Bind();
    pShader->Bind();
    CALL_GL_API(glDrawElements(GL_TRIANGLES, pEbo->GetCount(), GL_UNSIGNED_INT, (void *)0));
}
