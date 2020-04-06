#include "Texture.h"
#include "stb_image/stb_image.h"

/*
    Notice: cannot generate texture in game loop! for example, the following is illegal:

        Texture t1("foo.png");
        while()
        {
            t1.Bind(0);
            Texture t2("bar.png");  // wrong. since this will call glBindTexture() to current slot, which is 0, t1 will be replaced.
            t2.Bind(1);
        }
*/
Texture::Texture(const std::string &path):
    m_RendererID(0),
    m_FilePath(path),
    m_LocalBuffer(nullptr),
    m_Width(0),
    m_Height(0),
    m_BPP(0)
{
    stbi_set_flip_vertically_on_load(1);
    m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

    CALL_GL_API(glGenTextures(1, &m_RendererID));
    CALL_GL_API(glBindTexture(GL_TEXTURE_2D, m_RendererID));

    CALL_GL_API(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    CALL_GL_API(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    CALL_GL_API(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    CALL_GL_API(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    CALL_GL_API(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
    CALL_GL_API(glBindTexture(GL_TEXTURE_2D, 0));

    if (m_LocalBuffer)
    {
        stbi_image_free(m_LocalBuffer);
    }
}

Texture::~Texture()
{
    CALL_GL_API(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot) const
{
    CALL_GL_API(glActiveTexture(GL_TEXTURE0 + slot));
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Texture::Unbind(unsigned int slot)
{
    CALL_GL_API(glActiveTexture(GL_TEXTURE0 + slot));
    glBindTexture(GL_TEXTURE_2D, 0);
}
