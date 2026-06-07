//
// Created by kairav on 6/5/26.
//

#ifndef MAIN_GAME_TEXTURE2D_H
#define MAIN_GAME_TEXTURE2D_H
#include "BaseTexture.h"

class Texture2D : public BaseTexture {
public:
    Texture2D(const std::string& path);
    ~Texture2D();

    void bind(GLenum slot = GL_TEXTURE0) const override;

private:
    void loadFromFile(const std::string& path);
};



#endif //MAIN_GAME_TEXTURE2D_H
