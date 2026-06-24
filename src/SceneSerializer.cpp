//
// Created by kairav on 6/16/26.
//
#include <nlohmann/json.hpp>

#include "Entity.h"
using json = nlohmann::json;
#include "SceneSerializer.h"


void SceneSerializer::serialize(const std::string& path) const {
    json root;
    auto view = scene->getReg().view<IDComponent>();
    for (auto entity : view) {
        json entityJson;
        auto& id = view.get<IDComponent>(entity);
        entityJson["id"] = id.id;

        if(scene->getReg().all_of<TransformComponent>(entity)){
            auto& t = scene->getReg().get<TransformComponent>(entity);
            entityJson["Transform"] = {
                {"position",
                    {t.position.x,
                     t.position.y,
                     t.position.z}},
                {"rotation",
                    {t.orientation.w,
                     t.orientation.x,
                     t.orientation.y,
                     t.orientation.z}},
                {"scale",
                    {t.scale.x,
                     t.scale.y,
                     t.scale.z}}
            };
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
    for(auto& e : root["entities"]){
        Entity entity = scene->createEntity();
        entity.addComponent<IDComponent>(e["id"]);
        if(e.contains("Transform")){
            auto& t = entity.addComponent<TransformComponent>();
            auto pos = e["Transform"]["position"];
            t.position = {pos[0], pos[1], pos[2]};
            auto rot = e["Transform"]["rotation"];
            t.orientation = glm::quat(rot[0], rot[1], rot[2], rot[3]);
        }
    }
}
