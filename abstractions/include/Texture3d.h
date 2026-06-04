//
// Created by kairav on 6/5/26.
//

#ifndef MAIN_GAME_TEXTURE3D_H
#define MAIN_GAME_TEXTURE3D_H

#include "Texture.h"

class Texture3D : public Texture {
public:
    Texture3D(int width, int height, int depth, unsigned char* data);
    ~Texture3D();

    void bind(GLenum slot = GL_TEXTURE0) const override;

private:
    int m_width, m_height, m_depth;
};

#endif //MAIN_GAME_TEXTURE3D_H
