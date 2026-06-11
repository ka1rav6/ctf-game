//
// Created by kairav on 6/9/26.
//

#ifndef MAIN_GAME_RENDERER_H
#define MAIN_GAME_RENDERER_H
#include "../include/common.h"
#include "../include/Scene.h"
#include "../include/systems/PhysicsEngine.h"

class Camera;
class PhysicsEngine;

class Renderer {
public:
    static void render(Scene& scene, Camera& camera, PhysicsEngine* world, const glm::mat4& projection);
};

#endif //MAIN_GAME_RENDERER_H
