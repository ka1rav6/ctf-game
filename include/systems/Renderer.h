//
// Created by kairav on 6/9/26.
//

#ifndef MAIN_GAME_RENDERER_H
#define MAIN_GAME_RENDERER_H
#include "../include/common.h"
#include "../include/Scene.h"

class Camera;

class Renderer {
public:
    void render(Scene& scene, Camera& camera);
};


#endif //MAIN_GAME_RENDERER_H
