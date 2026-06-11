#ifndef MAIN_GAME_PHYSICSENGINE_H
#define MAIN_GAME_PHYSICSENGINE_H

#include <reactphysics3d/reactphysics3d.h>
#include "../include/Entity_Components.h"
class Scene;

class PhysicsEngine {
public:
    reactphysics3d::PhysicsCommon pCommon;
    reactphysics3d::PhysicsWorld* world = nullptr;
    float dt = 1.0f / 60.0f; // fixed timestep
    PhysicsEngine();
    ~PhysicsEngine();
    void update(Scene& scene);
    void syncToECS(Scene& scene);
};

#endif