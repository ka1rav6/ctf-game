#include "../include/Scene.h"
#include "../include/Entity.h"
#include "../include/Entity_Components.h"
#include "../include/systems/PhysicsEngine.h"
#include "../src/components/InfiniteGrid.h"
#include "../src/components/Sun.h"
#include "glm/trigonometric.hpp"

Scene::Scene(unsigned int cubeVAO) {
  createGround();
  createTree(glm::vec3(0.0f, 0.0f, 0.0f));
  createSun(cubeVAO);
  createGrid();
  for (int i = 0; i < 5; i++) {
    createTestCube(glm::vec3(1.0, 30.0f, (1.0f + 0.25 * i)));
  }
  LOG_INFO("Scene: all entities created");
}

Scene::~Scene() {
  for (auto& [key, shader] : shaderCache) {
    delete shader;
  }
  shaderCache.clear();

  auto sunView = reg.view<SunComponent>();
  for (auto e : sunView) {
    delete sunView.get<SunComponent>(e).sun;
    sunView.get<SunComponent>(e).sun = nullptr;
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
    mr.model = nullptr;
    mr.shader = nullptr;
  }

  LOG_WARN("Scene: all entity resources cleaned up");
}

Shader* Scene::getOrCreateShader(const std::string& vs, const std::string& fs) {
  std::string key = vs + "|" + fs;
  auto it = shaderCache.find(key);
  if (it != shaderCache.end()) return it->second;
  Shader* s = new Shader(vs, fs);
  shaderCache[key] = s;
  return s;
}

Entity Scene::createEntity() {
  auto e = reg.create();
  auto entity = Entity(e, this);
  entity.addComponent<IDComponent>(generateUUID());
  return entity;
}

void Scene::update(Camera &camera) {
  auto sunView = reg.view<SunComponent>();
  for (auto entity : sunView) {
    auto &sc = sunView.get<SunComponent>(entity);
    if (sc.sun) {
      sc.sun->update(camera);
    }
  }
  // engine.update() steps the physics world AND calls syncToECS()
  // internally (see PhysicsEngine.cpp), which copies every RigidBody's
  // transform back into its TransformComponent (position + Euler degrees).
  // That's the single source of truth for "mesh follows physics" - nothing
  // else in this file needs to duplicate that work.
  engine.update(*this);
}

Entity Scene::createGround() {
  Entity ground = createEntity();
  auto &transform = ground.addComponent<TransformComponent>();
  transform.position = {0.0f, 0.0f, 0.0f};
  auto &rb = ground.addComponent<RigidBodyComponent>();
  //auto &meshRenderer = ground.addComponent<MeshRendererComponent>();
  //meshRenderer.model = new Model("../external/models/cube/scene.gltf");
  //meshRenderer.shader = getOrCreateShader("../src/components/shaders/tree.vs",
  //                                        "../src/components/shaders/tree.fs");
  {
    using namespace reactphysics3d;
    // Ground mesh renders at transform.position.y == 0.0, so the collider's
    // top face needs to land exactly at y=0 too, or resting objects will
    // settle at the wrong height relative to what's drawn (this was off by
    // 0.25 units before: center at -9.55 + half-height 9.8 = top at 0.25,
    // not 0). Half-height stays 9.8; only the center's y changes.
    float halfHeight = 9.8f;
    Transform physicsTransform(Vector3(0, -halfHeight, 0), Quaternion::identity());
    rb.body = engine.world->createRigidBody(physicsTransform);
    rb.body->setType(BodyType::STATIC);
    CollisionShape *shape = engine.pCommon.createBoxShape(Vector3(1000000.0f, halfHeight, 1000000.0f));
    rb.body->addCollider(shape, Transform::identity());
  }
  // Ground is static and rb.body's spawn transform matches transform.position
  // above (both (0, -9.55, 0)) - syncToECS() will keep it pinned there even
  // though it never actually moves.
  return ground;
}

Entity Scene::createTree(glm::vec3 pos) {
  Entity tree = createEntity();
  tree.addComponent<TagComponent>(TagComponent{"Tree"});
  auto &transform = tree.addComponent<TransformComponent>();
  transform.position = pos;
  transform.orientation = glm::quat(glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f));
  transform.scale = glm::vec3(0.01f);
  auto &rb = tree.addComponent<RigidBodyComponent>();
  {
    using namespace reactphysics3d;
    Transform physicsTransform(
        Vector3(transform.position.x, transform.position.y, transform.position.z),
        Quaternion(transform.orientation.x, transform.orientation.y,
                   transform.orientation.z, transform.orientation.w));
    rb.body = engine.world->createRigidBody(physicsTransform);
    rb.body->setType(BodyType::STATIC);
    CollisionShape *shape = engine.pCommon.createCapsuleShape(1.0f, 3.0f);
    rb.body->addCollider(shape, Transform::identity());
  }
  auto &meshRenderer = tree.addComponent<MeshRendererComponent>();
  meshRenderer.meshPath = "../external/models/maple_tree/scene.gltf";
  meshRenderer.vertexPath = "../src/components/shaders/tree.vs";
  meshRenderer.fragmentPath = "../src/components/shaders/tree.fs";
  meshRenderer.model = new Model(meshRenderer.meshPath);
  meshRenderer.shader = getOrCreateShader(meshRenderer.vertexPath, meshRenderer.fragmentPath);
  return tree;
}

void Scene::clear() {
  auto sunView = reg.view<SunComponent>();
  for (auto e : sunView) {
    delete sunView.get<SunComponent>(e).sun;
    sunView.get<SunComponent>(e).sun = nullptr;
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
    mr.model = nullptr;
    mr.shader = nullptr;
  }

  for (auto& [key, shader] : shaderCache) {
    delete shader;
  }
  shaderCache.clear();

  reg.clear();
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
  Entity cube = createEntity();
  LOG_WARN("TEST CUBE CREATED");
  auto &transform = cube.addComponent<TransformComponent>();
  transform.position = pos;
  transform.scale = glm::vec3(1.0f);

  auto &meshRenderer = cube.addComponent<MeshRendererComponent>();
  meshRenderer.meshPath = "../external/models/dibbba.glb";
  meshRenderer.vertexPath = "../src/components/shaders/tree.vs";
  meshRenderer.fragmentPath = "../src/components/shaders/tree.fs";
  meshRenderer.model = new Model(meshRenderer.meshPath);
  meshRenderer.shader = getOrCreateShader(meshRenderer.vertexPath, meshRenderer.fragmentPath);

  auto &rb = cube.addComponent<RigidBodyComponent>();
  {
    using namespace reactphysics3d;
    Transform t(Vector3(pos.x, pos.y, pos.z), Quaternion::identity());
    rb.body = engine.world->createRigidBody(t);
    rb.body->setType(BodyType::DYNAMIC);
    // dibbba.glb measured at 2x2x2 (full size). createBoxShape takes
    // HALF-extents, so this must be 1x1x1, not 2x2x2 - the previous value
    // produced a collider twice the size of the visible mesh in every
    // dimension, which is why cubes appeared to float/clip/tilt at rest:
    // the (much bigger) invisible collider had already settled on the
    // ground while the small visible mesh rendered wherever its center
    // happened to be relative to that oversized box.
    CollisionShape *shape = engine.pCommon.createBoxShape(Vector3(1.0f, 1.0f, 1.0f));
    rb.body->addCollider(shape, Transform::identity());
  }
  // DYNAMIC body: transform.position/rotation set here is just the spawn
  // pose. From the first engine.update() call onward, syncToECS() overwrites
  // both every frame as the body falls/tumbles under gravity, so the mesh
  // tracks the physics body exactly with no extra code needed in this file.
  return cube;
}
