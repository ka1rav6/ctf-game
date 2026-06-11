//
// Created by kairav on 6/9/26.
//

#include "../include/Scene.h"
#include "../include/Entity.h"
#include "../include/Entity_Components.h"
#include "../include/systems/PhysicsEngine.h"
Scene::Scene() {
    // auto entity = reg.create();
    auto ground = createGround();


}

Entity Scene::createEntity(){
    auto e = reg.create();
    return Entity(e, this);
}

Entity Scene::createGround() {
    Entity ground = createEntity();
    auto& transform = ground.addComponent<TransformComponent>();
    transform.position = {0.0f, -1.0f, 0.0f};
    auto& rb = ground.addComponent<RigidbodyComponent>();
    using namespace reactphysics3d;
    Transform physicsTransform(
        Vector3(0, -1, 0),
        Quaternion::identity()
    );
    rb.body = physics.world->createRigidBody(physicsTransform);
    rb.body->setType(BodyType::STATIC);
    BoxShape* shape =
        physics.physicsCommon.createBoxShape(
            Vector3(100.0f, 1.0f, 100.0f)
        );
    rb.body->addCollider(
        shape,
        Transform::identity()
    );
    return ground;
}