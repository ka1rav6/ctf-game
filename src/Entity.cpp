//
// Created by kairav on 6/9/26.
//
#include "../include/Entity.h"
#include "../include/Scene.h"
#include "../include/common.h"
bool Entity::valid() const{
    return scene &&
           scene->reg.valid(handle);
}
template<typename T, typename... Args>
T& Entity::addComponent(Args&&... args){
    return scene->reg.emplace<T>(
        handle,
        std::forward<Args>(args)...);
}
template<typename T>
T& Entity::getComponent(){
    return scene->reg.get<T>(handle);
}
template<typename T>
bool Entity::hasComponent(){
    return scene->reg.all_of<T>(handle);
}
template<typename T>
void Entity::removeComponent(){
    scene->reg.remove<T>(handle);
}