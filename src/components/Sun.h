//
// Created by kairav on 6/7/26.
//

#ifndef MAIN_GAME_SUN_H
#define MAIN_GAME_SUN_H

#include "../include/common.h"
#include "../../include/common.h"

struct SunSettings
{
    glm::vec3 sunOffset = glm::vec3(150.0f, 120.0f, -150.0f);
    std::string vShaderFile = "../src/components/shaders/Sun.vs";
    std::string fShaderFile = "../src/components/shaders/Sun.fs";
    float scale = 20.0f;
};

class Sun {
private:
    SunSettings settings;  // ← declared FIRST so it's initialized before shader
    Shader shader; 
    glm::vec3 position;
    glm::mat4 model;
    unsigned int cubeVAO;
public:
    Sun(unsigned int cubeVAO, const SunSettings& settings = SunSettings());
    void update(Camera& camera);
    void render(Camera& camera, const glm::mat4& projection);
    glm::vec3 getPosition() const { return position; }
};

#endif // MAIN_GAME_SUN_H