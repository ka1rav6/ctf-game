//
// Created by kairav on 6/5/26.
//

#ifndef MAIN_GAME_TECTURECUBEMAP_H
#define MAIN_GAME_TECTURECUBEMAP_H

#include "Texture.h"


class TextureCubemap : public Texture {
public:
    TextureCubemap(const std::vector<std::string>& faces);
    ~TextureCubemap();

    void bind(GLenum slot = GL_TEXTURE0) const override;

private:
    void loadFaces(const std::vector<std::string>& faces);
};


#endif //MAIN_GAME_TECTURECUBEMAP_H
