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

    /* 
    buffer: data only, nothing else.
    layout: how to explain data.
    Basically, layout is something like:
        { {3 float unnormalized}, {3 float normalized}, {2 int unnormalized} }
    and with other details.
    */
    void AddBuffer(const VertexBuffer* pBuffer, const VertexBufferLayout* pLayout);
};