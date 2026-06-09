//
// Created by kairav on 6/9/26.
//

#ifndef MAIN_GAME_SCENE_H
#define MAIN_GAME_SCENE_H
#include "../include/common.h"
#include "../external/entt/entt.hpp"
#include "../include/Entity_Components.h"
class Entity;
class Scene {
public:
    Scene();
    ~Scene();
    Entity createEntity();
    entt::registry& getReg(){ return this->reg; };
private:
    entt::registry reg;
    friend class Entity;
};


#endif //MAIN_GAME_SCENE_H
