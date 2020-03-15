#pragma once
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"

class VertexArray
{
private:
    unsigned int m_RendererID;
public:
    VertexArray();
    ~VertexArray();

    inline unsigned int GetID() { return m_RendererID; }

    void Bind();
    void Unbind();

    void AddBuffer(const VertexBuffer& buffer, const VertexBufferLayout& layout);
};