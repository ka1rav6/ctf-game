//
// Created by kairav on 6/9/26.
//

#ifndef MAIN_GAME_ENTITY_COMPONENTS_H
#define MAIN_GAME_ENTITY_COMPONENTS_H

#include "../include/common.h"

struct Transform{
    glm::vec3 position{0.0f};
    glm::vec3 rotation{0.0f};
    glm::vec3 scale{1.0f};
    Shader* shader;
};
struct IDComponent{
    uint64_t id;
};
struct TagComponent{
    std::string name;
};

struct MeshRenderer{
    std::string meshPath;
    Shader* shader;
    Model* model;
};

struct Rigidbody{
    float mass = 1.0f;
};
struct Collider{};
struct Physics{};
struct Light{};
struct Sprite{};
struct Audio{};

#endif //MAIN_GAME_ENTITY_COMPONENTS_H
