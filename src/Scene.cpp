//
// Created by kairav on 6/9/26.
//

#include "../include/Scene.h"
#include "../include/Entity.h"
#include "../include/Entity_Components.h"
#include "../include/systems/PhysicsEngine.h"

// Sun and InfiniteGrid were forward-declared in Entity_Components.h.
// We need the full definitions here because we construct them.
#include "../src/components/InfiniteGrid.h"
#include "../src/components/Sun.h"

Scene::Scene(unsigned int cubeVAO) {
  // Creates each game entity with its components.
  createGround();
  createTree(glm::vec3(0.0f, 0.0f, 0.0f));
  createSun(cubeVAO);
  createGrid();
  for (float i = 0.0f; i < 10.0f; i++) {
    createTestCube(glm::vec3(i, 15.0f, 0.0f));
  }
  for (float i = 0.0f; i < 10.0f; i++) {
    createTestCube(glm::vec3(0.0f, 15.0f, i));
  }
  LOG_INFO("Scene: all entities created");
}

Scene::~Scene() {
  auto sunView = reg.view<SunComponent>();
  for (auto e : sunView) {
    delete sunView.get<SunComponent>(e).sun;
    sunView.get<SunComponent>(e).sun =
        nullptr; // removing dangling ptrs coz i have ocd
  }
  auto gridView = reg.view<InfiniteGridComponent>();
  for (auto e : gridView) {
    delete gridView.get<InfiniteGridComponent>(e).grid;
    gridView.get<InfiniteGridComponent>(e).grid = nullptr;
  }

  auto meshView = reg.view<MeshRendererComponent>();
  for (auto e : meshView) {
    auto &mr = meshView.get<MeshRendererComponent>(e);
    delete mr.model;
    delete mr.shader;
    mr.model = nullptr;
    mr.shader = nullptr;
  }
  LOG_WARN("Scene: all entity resources cleaned up");
}

Entity Scene::createEntity() {
  auto e = reg.create();
  return Entity(e, this);
}
void Scene::update(Camera &camera) {
  // so it always appears at a fixed offset in the sky.
  auto sunView = reg.view<SunComponent>();
  for (auto entity : sunView) {
    auto &sc = sunView.get<SunComponent>(entity);
    if (sc.sun) {
      sc.sun->update(camera);
    }
  }
  engine.update(*this); // every frame
}
Entity Scene::createGround() {
  Entity ground = createEntity();
  auto &transform = ground.addComponent<TransformComponent>();
  transform.position = {0.0f, 0.0f, 0.0f};
  auto &rb = ground.addComponent<RigidBodyComponent>();
  using namespace reactphysics3d;
  Transform physicsTransform(Vector3(0, -1, 0), Quaternion::identity());
  rb.body = engine.world->createRigidBody(physicsTransform);
  rb.body->setType(BodyType::STATIC);

  CollisionShape *shape =
      engine.pCommon.createBoxShape(Vector3(100.0f, 1.0f, 100.0f));
  rb.body->addCollider(shape, Transform::identity());
  return ground;
}
Entity Scene::createTree(glm::vec3 pos) {
  Entity tree = createEntity();
  tree.addComponent<TagComponent>(TagComponent{"Tree"});
  auto &transform = tree.addComponent<TransformComponent>();
  transform.position = pos;
  transform.rotation = glm::vec3(-90.0f, 0.0f, 0.0f);
  transform.scale = glm::vec3(0.01f);
  // RIGID BODY PHYSICS
  auto &rb = tree.addComponent<RigidBodyComponent>();
  using namespace reactphysics3d;
  Transform physicsTransform(
      Vector3(transform.position.x, transform.position.y, transform.position.z),
      Quaternion::identity());
  rb.body = engine.world->createRigidBody(physicsTransform);
  rb.body->setType(BodyType::STATIC);
  // approximate tree collider (simple for now)
  CollisionShape *shape = engine.pCommon.createCapsuleShape(1.0f, 3.0f);
  rb.body->addCollider(shape, Transform::identity());
  auto &meshRenderer = tree.addComponent<MeshRendererComponent>();
  meshRenderer.model = new Model("../external/models/maple_tree/scene.gltf");
  meshRenderer.shader = new Shader("../src/components/shaders/tree.vs",
                                   "../src/components/shaders/tree.fs");
  return tree;
}
Entity Scene::createSun(unsigned int cubeVAO) {
  Entity sunEntity = createEntity();
  sunEntity.addComponent<TagComponent>(TagComponent{"Sun"});
  auto &sc = sunEntity.addComponent<SunComponent>();
  sc.sun = new Sun(cubeVAO);

  LOG_INFO("Scene: Sun entity created");
  return sunEntity;
}

Entity Scene::createGrid() {
  Entity gridEntity = createEntity();
  gridEntity.addComponent<TagComponent>(TagComponent{"Grid"});
  auto &gc = gridEntity.addComponent<InfiniteGridComponent>();
  gc.grid = new InfiniteGrid();
  LOG_INFO("Scene: Grid entity created");
  return gridEntity;
}
Entity Scene::createTestCube(glm::vec3 pos) {
  // std::string warnStr ="Scene: Cube entity created" + std::to_string(pos.x) +
  // " " + std::to_string(pos.y) + " " + std::to_string(pos.z);
  // LOG_WARN(warnStr);
  Entity cube = createEntity();
  LOG_WARN("TEST CUBE CREATED");
  auto &transform = cube.addComponent<TransformComponent>();
  transform.position = pos;
  transform.scale = glm::vec3(0.1f);

  auto &meshRenderer = cube.addComponent<MeshRendererComponent>();
  meshRenderer.model =
      new Model("../external/models/cube/scene.gltf"); // or any simple cube
  meshRenderer.shader = new Shader("../src/components/shaders/tree.vs",
                                   "../src/components/shaders/tree.fs");

  auto &rb = cube.addComponent<RigidBodyComponent>();
  using namespace reactphysics3d;
  Transform t(Vector3(pos.x, pos.y, pos.z), Quaternion::identity());
  rb.body = engine.world->createRigidBody(t);
  rb.body->setType(BodyType::DYNAMIC);
  CollisionShape *shape =
      engine.pCommon.createBoxShape(Vector3(0.5f, 0.5f, 0.5f));
  rb.body->addCollider(shape, Transform::identity());
  return cube;
}
