//
// Created by kairav on 6/9/26.
//

#ifndef MAIN_GAME_SCENE_H
#define MAIN_GAME_SCENE_H
#include "../include/common.h"
#include "../external/entt/entt.hpp"
#include "../include/Entity_Components.h"
#include "../include/systems/PhysicsEngine.h"
#include <unordered_map>
#include <string>
class Entity;
class Scene {
public:
    Scene(unsigned int cubeVAO);
    ~Scene();
    Entity createEntity();
    void update(Camera& camera);
    entt::registry& getReg() {
        return this->reg;
    };
    PhysicsEngine engine;
    Shader* getOrCreateShader(const std::string& vs, const std::string& fs);
    void clear();
private:
    entt::registry reg;
    Entity createGround();
    Entity createTree(glm::vec3 pos);
    Entity createTestCube(glm::vec3 pos);
    Entity createSun(unsigned int cubeVAO);
    Entity createGrass(glm::vec3);
    Entity createGrid();
    std::unordered_map<std::string, Shader*> shaderCache;
    friend class Entity;
};

#endif //MAIN_GAME_SCENE_H
