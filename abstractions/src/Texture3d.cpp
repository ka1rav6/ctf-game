#include "../include/Texture3d.h"


Texture3D::Texture3D(int width, int height, int depth, unsigned char* data)
    : m_width(width), m_height(height), m_depth(depth)
{
    glGenTextures(1, &m_ID);
    glBindTexture(GL_TEXTURE_3D, m_ID);

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage3D(
        GL_TEXTURE_3D,
        0,
        GL_RGBA,
        width,
        height,
        depth,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        data
    );
}
Texture3D::~Texture3D()
{
    glDeleteTextures(1, &m_ID);
}
void Texture3D::bind(GLenum slot) const
{
    glActiveTexture(slot);
    glBindTexture(GL_TEXTURE_3D, m_ID);
}
