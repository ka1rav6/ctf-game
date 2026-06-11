//
// Created by kairav on 6/9/26.
//


#include "../include/Entity.h"
#include "../include/Scene.h"
#include "../include/common.h"

bool Entity::valid() const{
    return scene && scene->reg.valid(handle);
}

Entity::Entity(entt::entity e, Scene* scene) {
    this->scene = scene;
    this->handle = e;
}