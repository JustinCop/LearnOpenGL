#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const void *data, unsigned int size)
{
    CALL_GL_API(glGenBuffers(1, &m_RendererID));
    CALL_GL_API(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    CALL_GL_API(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
    CALL_GL_API(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

IndexBuffer::~IndexBuffer()
{
    CALL_GL_API(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind()
{
    CALL_GL_API(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind()
{
    CALL_GL_API(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
