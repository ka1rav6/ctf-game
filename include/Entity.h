//
// Created by kairav on 6/9/26.
//

#ifndef MAIN_GAME_ENTITY_H
#define MAIN_GAME_ENTITY_H
#include "../include/common.h"

class Scene;
class Entity {
public:
    Entity() = default;
    Entity(entt::entity, Scene* scene);
    ~Entity() = default;
    bool valid() const;
    template<typename T, typename... Args>
    T& addComponent(Args&&... args);
    template<typename T>
    T& getComponent();
    template<typename T>
    bool hasComponent() const;
    template<typename T>
    void removeComponent() const;

private:
    entt::entity handle{entt::null};
    Scene* scene = nullptr;
};


#include "../include/Scene.h"
template<typename T, typename... Args>
T& Entity::addComponent(Args&&... args){
    return scene->reg.emplace<T>(handle,std::forward<Args>(args)...);
}

template<typename T>
T& Entity::getComponent(){
    return scene->reg.get<T>(handle);
}
template<typename T>
bool Entity::hasComponent() const {
    return scene->reg.all_of<T>(handle);
}
template<typename T>
void Entity::removeComponent() const {
    scene->reg.remove<T>(handle);
}

#endif //MAIN_GAME_ENTITY_H
