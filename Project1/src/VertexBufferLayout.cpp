#include "VertexBufferLayout.h"

VertexBufferLayout::VertexBufferLayout():
    m_Stride(0),
    m_Elements(0)
{
}

VertexBufferLayout::~VertexBufferLayout()
{
}

void VertexBufferLayout::Push(unsigned int count, GLenum type)
{
    m_Elements.push_back({ count, type, GL_FALSE });
    switch (type)
    {
    case GL_FLOAT:
        m_Stride += count * sizeof(float);
        break;
    default:
        assert(false);
    }
}

