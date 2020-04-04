#pragma once
class IndexBuffer
{
private:
    unsigned int m_RendererID;
    unsigned int m_Count;
public:
    IndexBuffer(const void *data, unsigned int size);
    ~IndexBuffer();

    inline unsigned int GetID() const { return m_RendererID; }
    inline unsigned int GetCount() const { return m_Count; }

    void Bind();
    void Unbind();
};