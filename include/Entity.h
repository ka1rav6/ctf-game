//
// Created by kairav on 6/9/26.
//

#ifndef MAIN_GAME_ENTITY_H
#define MAIN_GAME_ENTITY_H
#include "../include/common.h"
class Scene;
class Entity {
    Entity() = default;
    Entity(entt::entity, Scene* scene);
    ~Entity() = default;
    bool valid() const;

    template<typename T, typename... Args>
    T& addComponent(Args&&... args);
    template<typename T>
    T& getComponent();
    template<typename T>
    bool hasComponent();
    template<typename T>
    void removeComponent();
private:
    entt::entity handle{entt::null};
    Scene* scene = nullptr;
};


#endif //MAIN_GAME_ENTITY_H
