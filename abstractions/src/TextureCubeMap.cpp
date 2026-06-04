#include "../include/TextureCubeMap.h"



TextureCubemap::TextureCubemap(const std::vector<std::string>& faces)
{
    glGenTextures(1, &m_ID);
    loadFaces(faces);
}
TextureCubemap::~TextureCubemap()
{
    glDeleteTextures(1, &m_ID);
}
void TextureCubemap::bind(GLenum slot) const
{
    glActiveTexture(slot);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);
}
void TextureCubemap::loadFaces(const std::vector<std::string>& faces)
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    int w, h, c;

    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &w, &h, &c, 0);

        if (!data) {
            std::cout << "Cubemap load failed: " << faces[i] << std::endl;
            continue;
        }

        GLenum format = (c == 4) ? GL_RGBA : GL_RGB;

        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0,
            format,
            w,
            h,
            0,
            format,
            GL_UNSIGNED_BYTE,
            data
        );

        stbi_image_free(data);
    }
}
