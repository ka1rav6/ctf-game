//
// Created by kairav on 6/11/26.
//

#include "PhysicsEngine.h"
// PhysicsSystem.cpp

PhysicsEngine::PhysicsEngine() {
    reactphysics3d::PhysicsWorld::WorldSettings settings;
    settings.gravity = reactphysics3d::Vector3(0, -9.81f, 0);
    world = physicsCommon.createPhysicsWorld(settings);
}

void PhysicsEngine::update(float dt) {
    world->update(dt);
}