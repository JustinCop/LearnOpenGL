#pragma once
#include <vector>
#include <assert.h>
#include <GL/glew.h>


struct VertexBufferElement
{
    unsigned int count; // number of scalar per vertex
    GLenum type;        // data type
    bool normalized;

    unsigned int GetTypeSize() const
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

    template<typename T>
    void Push(unsigned int count)
    {
        assert(false);
    }

    template<>
    void Push<float>(unsigned int count)
    {
        m_Elements.push_back({ count, GL_FLOAT, GL_FALSE });
        m_Stride += count * sizeof(float);
    }


    auto GetElements() const { return m_Elements; }
    unsigned int GetStride() const { return m_Stride; }
};