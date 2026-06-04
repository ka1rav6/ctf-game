//
// Created by kairav on 6/5/26.
//

#ifndef MAIN_GAME_TEXTURE_H
#define MAIN_GAME_TEXTURE_H
#include "common.h"

class Texture {
public:
    virtual ~Texture() = default;

    virtual void bind(GLenum slot = GL_TEXTURE0) const = 0;
    unsigned int id() const { return m_ID; }

protected:
    unsigned int m_ID = 0;
};


#endif //MAIN_GAME_TEXTURE_H
