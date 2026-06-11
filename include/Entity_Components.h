//
// Created by kairav on 6/9/26.
//
// ─── ECS COMPONENT DEFINITIONS ──────────────────────────────────────────────────
// Every "thing" in the game world is an Entity (just an integer ID).
// Entities have no data or behaviour of their own — they get their meaning
// from the Components attached to them.  Each struct below is one component
// type.  Systems (like the Renderer) query the registry for entities that
// own a particular combination of components and act on them.
// ─────────────────────────────────────────────────────────────────────────────────

#ifndef MAIN_GAME_ENTITY_COMPONENTS_H
#define MAIN_GAME_ENTITY_COMPONENTS_H

#include "../include/common.h"

// Forward declarations
class Sun;
class InfiniteGrid;

struct TransformComponent{
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

struct MeshRendererComponent{
    std::string meshPath;
    Shader* shader;
    Model* model;
};

struct SunComponent {
    Sun* sun = nullptr;
};

struct InfiniteGridComponent {
    InfiniteGrid* grid = nullptr;
};

// Physics components
struct RigidBodyComponent {
    reactphysics3d::RigidBody* body = nullptr;
    reactphysics3d::Vector3 position{};
    reactphysics3d::Quaternion orientation= reactphysics3d::Quaternion::identity();
    reactphysics3d::Transform transform{};
};
struct BoxColliderComponent {
    glm::vec3 halfExtents;
};

struct Collider{};
struct Light{};
struct Sprite{};
struct Audio{};

#endif //MAIN_GAME_ENTITY_COMPONENTS_H
