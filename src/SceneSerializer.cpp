#include <fstream>
#include <nlohmann/json.hpp>

#include "Entity.h"
#include "Entity_Components.h"
#include "SceneSerializer.h"
#include "components/Sun.h"
#include "components/InfiniteGrid.h"

using json = nlohmann::json;

SceneSerializer::SceneSerializer(Scene* scene, unsigned int cubeVAO)
    : scene(scene), cubeVAO(cubeVAO) {}

void SceneSerializer::serialize(const std::string& path) const {
    json root;
    auto view = scene->getReg().view<IDComponent>();
    for (auto entity : view) {
        json entityJson;
        auto& id = view.get<IDComponent>(entity);
        entityJson["id"] = id.id;

        if (scene->getReg().all_of<TagComponent>(entity)) {
            auto& tag = scene->getReg().get<TagComponent>(entity);
            entityJson["Tag"]["name"] = tag.name;
        }

        if (scene->getReg().all_of<TransformComponent>(entity)) {
            auto& t = scene->getReg().get<TransformComponent>(entity);
            entityJson["Transform"] = {
                {"position", {t.position.x, t.position.y, t.position.z}},
                {"rotation", {t.orientation.w, t.orientation.x, t.orientation.y, t.orientation.z}},
                {"scale", {t.scale.x, t.scale.y, t.scale.z}}
            };
        }

        if (scene->getReg().all_of<MeshRendererComponent>(entity)) {
            auto& mr = scene->getReg().get<MeshRendererComponent>(entity);
            entityJson["MeshRenderer"] = {
                {"meshPath", mr.meshPath},
                {"vertexPath", mr.vertexPath},
                {"fragmentPath", mr.fragmentPath}
            };
        }

        if (scene->getReg().all_of<RigidBodyComponent>(entity)) {
            auto& rb = scene->getReg().get<RigidBodyComponent>(entity);
            std::string typeStr = "STATIC";
            if (rb.bodyType == reactphysics3d::BodyType::DYNAMIC) typeStr = "DYNAMIC";
            else if (rb.bodyType == reactphysics3d::BodyType::KINEMATIC) typeStr = "KINEMATIC";
            entityJson["RigidBody"] = {
                {"bodyType", typeStr},
                {"mass", rb.mass}
            };
        }

        if (scene->getReg().all_of<BoxColliderComponent>(entity)) {
            auto& bc = scene->getReg().get<BoxColliderComponent>(entity);
            entityJson["BoxCollider"] = {
                {"halfExtents", {bc.halfExtents.x, bc.halfExtents.y, bc.halfExtents.z}}
            };
        }

        if (scene->getReg().all_of<SunComponent>(entity)) {
            entityJson["SunComponent"] = true;
        }

        if (scene->getReg().all_of<InfiniteGridComponent>(entity)) {
            entityJson["InfiniteGridComponent"] = true;
        }

        root["entities"].push_back(entityJson);
    }
    std::ofstream file(path);
    file << root.dump(2);
}

void SceneSerializer::deserialize(const std::string& path) const {
    std::ifstream file(path);
    json root;
    file >> root;

    scene->clear();

    for (auto& e : root["entities"]) {
        Entity entity = scene->createEntity();
        entity.addComponent<IDComponent>(e["id"]);

        if (e.contains("Tag")) {
            entity.addComponent<TagComponent>(e["Tag"]["name"]);
        }

        if (e.contains("Transform")) {
            auto& t = entity.addComponent<TransformComponent>();
            auto pos = e["Transform"]["position"];
            t.position = {pos[0], pos[1], pos[2]};
            auto rot = e["Transform"]["rotation"];
            t.orientation = glm::quat(static_cast<float>(rot[0]), static_cast<float>(rot[1]),
                                      static_cast<float>(rot[2]), static_cast<float>(rot[3]));
            auto scale = e["Transform"]["scale"];
            t.scale = {scale[0], scale[1], scale[2]};
        }

        if (e.contains("MeshRenderer")) {
            auto& mr = entity.addComponent<MeshRendererComponent>();
            mr.meshPath = e["MeshRenderer"]["meshPath"];
            mr.vertexPath = e["MeshRenderer"]["vertexPath"];
            mr.fragmentPath = e["MeshRenderer"]["fragmentPath"];
            mr.model = new Model(mr.meshPath);
            mr.shader = scene->getOrCreateShader(mr.vertexPath, mr.fragmentPath);
        }

        if (e.contains("RigidBody")) {
            using namespace reactphysics3d;
            auto& rb = entity.addComponent<RigidBodyComponent>();
            rb.mass = e["RigidBody"]["mass"];
            std::string typeStr = e["RigidBody"]["bodyType"];
            if (typeStr == "DYNAMIC") rb.bodyType = BodyType::DYNAMIC;
            else if (typeStr == "KINEMATIC") rb.bodyType = BodyType::KINEMATIC;
            else rb.bodyType = BodyType::STATIC;

            Transform physicsTransform = Transform::identity();
            if (entity.hasComponent<TransformComponent>()) {
                auto& t = entity.getComponent<TransformComponent>();
                physicsTransform = Transform(
                    Vector3(t.position.x, t.position.y, t.position.z),
                    Quaternion(t.orientation.x, t.orientation.y,
                               t.orientation.z, t.orientation.w));
            }
            rb.body = scene->engine.world->createRigidBody(physicsTransform);
            rb.body->setType(rb.bodyType);
        }

        if (e.contains("BoxCollider")) {
            auto& bc = entity.addComponent<BoxColliderComponent>();
            auto half = e["BoxCollider"]["halfExtents"];
            bc.halfExtents = {half[0], half[1], half[2]};

            if (entity.hasComponent<RigidBodyComponent>()) {
                auto& rb = entity.getComponent<RigidBodyComponent>();
                if (rb.body) {
                    using namespace reactphysics3d;
                    Vector3 he(bc.halfExtents.x, bc.halfExtents.y, bc.halfExtents.z);
                    CollisionShape* shape = scene->engine.pCommon.createBoxShape(he);
                    rb.body->addCollider(shape, Transform::identity());
                }
            }
        }

        if (e.contains("SunComponent")) {
            auto& sc = entity.addComponent<SunComponent>();
            sc.sun = new Sun(cubeVAO);
        }

        if (e.contains("InfiniteGridComponent")) {
            entity.addComponent<InfiniteGridComponent>().grid = new InfiniteGrid();
        }
    }
}
