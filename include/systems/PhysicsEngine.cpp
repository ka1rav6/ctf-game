//
// Created by kairav on 6/11/26.
//

#include "PhysicsEngine.h"
#include "../include/Scene.h"
#include "../include/Entity_Components.h"
// PhysicsSystem.cpp

PhysicsEngine::PhysicsEngine() {
    reactphysics3d::PhysicsWorld::WorldSettings settings;
    settings.gravity = reactphysics3d::Vector3(0, -9.81f, 0);
    settings.worldName = "CTF BYLD";
    world = pCommon.createPhysicsWorld(settings);
    auto sphereShape = pCommon.createSphereShape(10);
}

void PhysicsEngine::update(Scene& scene) {
    world->update(dt);
    syncToECS(scene);
}
PhysicsEngine::~PhysicsEngine() {
    pCommon.destroyPhysicsWorld(world);
}

void PhysicsEngine::syncToECS(Scene& scene) {
    auto& registry = scene.getReg();
    auto view = registry.view<TransformComponent, RigidBodyComponent>();
    for (auto entity : view) {
        auto& transform = view.get<TransformComponent>(entity);
        auto& rb = view.get<RigidBodyComponent>(entity);
        if (!rb.body) continue;
        using namespace reactphysics3d;
        Transform t = rb.body->getTransform();
        Vector3 p = t.getPosition();
        Quaternion q = t.getOrientation();
        transform.position = {p.x, p.y, p.z};
        transform.orientation = glm::quat(q.w, q.x, q.y, q.z);
    }
}