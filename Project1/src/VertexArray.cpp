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

void VertexArray::AddBuffer(const VertexBuffer *pBuffer, const VertexBufferLayout *pLayout)
{
    Bind();
    pBuffer->Bind();
    const auto &elements = pLayout->GetElements();
    unsigned int offset = 0;

    /* 
    here, shader must set
        layout (location = 0) in
    and increase location one by one, because we don't want app to specify input location. Let the framework do this.
    */
    for (unsigned int i = 0; i < elements.size(); i++)
    {
        CALL_GL_API(glEnableVertexAttribArray(i));
        CALL_GL_API(glVertexAttribPointer(i, elements[i].count, elements[i].type, elements[i].normalized, pLayout->GetStride(), (void *)offset));
        offset += elements[i].count * elements[i].GetTypeSize();
    }
}

