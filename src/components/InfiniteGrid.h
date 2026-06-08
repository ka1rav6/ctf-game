//
// Created by kairav on 6/8/26.
//

#ifndef MAIN_GAME_INFINITEGRID_H
#define MAIN_GAME_INFINITEGRID_H
#include "../include/common.h"
#include "../include/Logger.h"
class InfiniteGrid {
public:
    InfiniteGrid();
    ~InfiniteGrid();
    void render(Camera& camera, const glm::mat4& projection, float nearPlane, float farPlane);
private:
    Shader* shader = nullptr;
    unsigned int VAO{};   // empty VAO
};


#endif //MAIN_GAME_INFINITEGRID_H
