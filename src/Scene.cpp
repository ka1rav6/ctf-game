//
// Created by kairav on 6/9/26.
//

#include "../include/Scene.h"
#include "../include/Entity.h"
#include "../include/Entity_Components.h"
#include "../include/systems/PhysicsEngine.h"

// Sun and InfiniteGrid were forward-declared in Entity_Components.h.
// We need the full definitions here because we construct them.
#include "../src/components/Sun.h"
#include "../src/components/InfiniteGrid.h"

Scene::Scene(unsigned int cubeVAO) {
    // Creates each game entity with its components.
    createGround();
    createTree();
    createSun(cubeVAO);
    createGrid();
    LOG_INFO("Scene: all entities created");
}

Scene::~Scene() {
    auto sunView = reg.view<SunComponent>();
    for (auto e : sunView) {
        delete sunView.get<SunComponent>(e).sun;
        sunView.get<SunComponent>(e).sun = nullptr; // removing dangling ptrs coz i have ocd
    }
    auto gridView = reg.view<InfiniteGridComponent>();
    for (auto e : gridView) {
        delete gridView.get<InfiniteGridComponent>(e).grid;
        gridView.get<InfiniteGridComponent>(e).grid = nullptr;
    }

    auto meshView = reg.view<MeshRendererComponent>();
    for (auto e : meshView) {
        auto& mr = meshView.get<MeshRendererComponent>(e);
        delete mr.model;
        delete mr.shader;
        mr.model = nullptr;
        mr.shader = nullptr;
    }
    LOG_WARN("Scene: all entity resources cleaned up");
}

Entity Scene::createEntity(){
    auto e = reg.create();
    return Entity(e, this);
}
void Scene::update(Camera& camera) {
    // Update all Sun entities — each Sun tracks the camera position
    // so it always appears at a fixed offset in the sky.
    auto sunView = reg.view<SunComponent>();
    for (auto entity : sunView) {
        auto& sc = sunView.get<SunComponent>(entity);
        if (sc.sun) {
            sc.sun->update(camera);
        }
    }
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
    BoxShape* shape = physics.physicsCommon.createBoxShape(Vector3(100.0f, 1.0f, 100.0f) );
    rb.body->addCollider( shape, Transform::identity());
    return ground;
}
Entity Scene::createTree() {
    Entity tree = createEntity();
    tree.addComponent<TagComponent>(TagComponent{"Tree"});
    auto& transform = tree.addComponent<TransformComponent>();
    transform.position = glm::vec3(0.0f);
    transform.rotation = glm::vec3(-90.0f, 0.0f, 0.0f);
    transform.scale    = glm::vec3(0.01f);

    // Load the model and create its shader
    auto& meshRenderer = tree.addComponent<MeshRendererComponent>();
    meshRenderer.model  = new Model("../external/models/maple_tree/scene.gltf");
    meshRenderer.shader = new Shader("../src/components/shaders/tree.vs",
                                     "../src/components/shaders/tree.fs");
    glm::vec3 min(FLT_MAX);
    glm::vec3 max(-FLT_MAX);
    for (auto& mesh : meshRenderer.model->meshes) {
        for (auto& v : mesh.vertices) {
            min = glm::min(min, v.Position);
            max = glm::max(max, v.Position);
        }
    }
    std::cout << "Min: " << min.x << " " << min.y << " " << min.z << '\n';
    std::cout << "Max: " << max.x << " " << max.y << " " << max.z << '\n';
    LOG_INFO("Scene: Tree entity created");
    return tree;
}
Entity Scene::createSun(unsigned int cubeVAO) {
    Entity sunEntity = createEntity();
    sunEntity.addComponent<TagComponent>(TagComponent{"Sun"});
    auto& sc = sunEntity.addComponent<SunComponent>();
    sc.sun = new Sun(cubeVAO);

    LOG_INFO("Scene: Sun entity created");
    return sunEntity;
}

Entity Scene::createGrid() {
    Entity gridEntity = createEntity();
    gridEntity.addComponent<TagComponent>(TagComponent{"Grid"});
    auto& gc = gridEntity.addComponent<InfiniteGridComponent>();
    gc.grid = new InfiniteGrid();
    LOG_INFO("Scene: Grid entity created");
    return gridEntity;
}