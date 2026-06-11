//
// Created by kairav on 6/9/26.
//

#ifndef MAIN_GAME_SCENE_H
#define MAIN_GAME_SCENE_H
#include "../include/common.h"
#include "../external/entt/entt.hpp"
#include "../include/Entity_Components.h"
#include "../include/systems/PhysicsEngine.h"

class Entity;
class Scene {
public:
    Scene(unsigned int cubeVAO);
    ~Scene();
    Entity createEntity();
    void update(Camera& camera);
    entt::registry& getReg() {
        return this->reg;
    };
    PhysicsEngine engine;
private:
    entt::registry reg;
    Entity createGround();
    Entity createTree();
    Entity createTestCube();
    Entity createSun(unsigned int cubeVAO);
    Entity createGrid();
    friend class Entity;
};

#endif //MAIN_GAME_SCENE_H
