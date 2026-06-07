//
// Created by kairav on 6/5/26.
//

#ifndef MAIN_GAME_TECTURECUBEMAP_H
#define MAIN_GAME_TECTURECUBEMAP_H

#include "BaseTexture.h"


class TextureCubemap : public BaseTexture {
public:
    TextureCubemap(const std::vector<std::string>& faces);
    ~TextureCubemap();

    void bind(GLenum slot = GL_TEXTURE0) const override;

private:
    void loadFaces(const std::vector<std::string>& faces);
};


#endif //MAIN_GAME_TECTURECUBEMAP_H
