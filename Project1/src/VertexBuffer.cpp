#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    CALL_GL_API(glGenBuffers(1, &m_RendererID));
    CALL_GL_API(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    CALL_GL_API(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
    CALL_GL_API(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

VertexBuffer::~VertexBuffer()
{
    CALL_GL_API(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
    CALL_GL_API(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
    CALL_GL_API(glBindBuffer(GL_ARRAY_BUFFER, 0));
}


