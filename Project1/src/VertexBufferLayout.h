#pragma once
#include <vector>
#include <assert.h>
#include <GL/glew.h>


struct VertexBufferElement
{
    unsigned int count;
    GLenum type;
    bool normalized;

    static unsigned int GetSizeOfType(GLenum type)
    {
        switch (type)
        {
        case GL_FLOAT:
            return sizeof(float);
        default:
            assert(false);
        }
    }
};

class VertexBufferLayout
{
private:
    unsigned int m_Stride;
    std::vector<VertexBufferElement> m_Elements;

public:
    VertexBufferLayout();
    ~VertexBufferLayout();

    void Push(unsigned int count, GLenum type);
    auto GetElements() const { return m_Elements; }
    unsigned int GetStride() const { return m_Stride; }
};