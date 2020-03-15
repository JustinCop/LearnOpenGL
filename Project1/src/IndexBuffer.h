#pragma once
class IndexBuffer
{
private:
    unsigned int m_RendererID;
public:
    IndexBuffer(const void *data, unsigned int size);
    ~IndexBuffer();

    inline unsigned int GetID() { return m_RendererID; }

    void Bind();
    void Unbind();
};