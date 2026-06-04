#include "../include/Texture2d.h"


Texture2D::Texture2D(const std::string& path)
{
    glGenTextures(1, &m_ID);
    loadFromFile(path);
}

Texture2D::~Texture2D()
{
    glDeleteTextures(1, &m_ID);
}

void Texture2D::bind(GLenum slot) const
{
    glActiveTexture(slot);
    glBindTexture(GL_TEXTURE_2D, m_ID);
}

void Texture2D::loadFromFile(const std::string& path)
{
    glBindTexture(GL_TEXTURE_2D, m_ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    int w, h, c;
    unsigned char* data = stbi_load(path.c_str(), &w, &h, &c, 0);

    if (!data) {
        std::cout << "Failed to load texture2D: " << stbi_failure_reason() << std::endl;
        return;
    }

    GLenum format = GL_RGB;
    if (c == 1) format = GL_RED;
    else if (c == 3) format = GL_RGB;
    else if (c == 4) format = GL_RGBA;

    glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
}