#include "VertexArray.h"
#include "Renderer.h"
#include "GL/glew.h"

VertexArray::VertexArray()
{
    CALL_GL_API(glGenVertexArrays(1, &m_RendererID));
    /*CALL_GL_API(glBindVertexArray(m_RendererID));*/
}

VertexArray::~VertexArray()
{
    CALL_GL_API(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::Bind()
{
    CALL_GL_API(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind()
{
    CALL_GL_API(glBindVertexArray(0));
}

void VertexArray::AddBuffer(const VertexBuffer& buffer, const VertexBufferLayout& layout)
{
    Bind();
    buffer.Bind();
    const auto &elements = layout.GetElements();
    unsigned int offset = 0;

    for (unsigned int i = 0; i < elements.size(); i++)
    {
        CALL_GL_API(glEnableVertexAttribArray(i));
        CALL_GL_API(glVertexAttribPointer(i, elements[i].count, elements[i].type, elements[i].normalized, layout.GetStride(), (void *)offset));
        offset += elements[i].count * elements[i].GetTypeSize();
    }
    buffer.Unbind();
    Unbind();
}

