//
// Created by kairav on 6/11/26.
//

#ifndef MAIN_GAME_PHYSICSENGINE_H
#define MAIN_GAME_PHYSICSENGINE_H

// PhysicsSystem.h

#include <reactphysics3d/reactphysics3d.h>

namespace reactphysics3d {
    class PhysicsWorld;
}

class PhysicsEngine {
public:
    reactphysics3d::PhysicsCommon physicsCommon;
    reactphysics3d::PhysicsWorld* world;

    PhysicsEngine();
    void update(float dt);
};



#endif //MAIN_GAME_PHYSICSENGINE_H
