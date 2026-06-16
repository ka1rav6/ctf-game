//
// Created by kairav on 6/16/26.
//

#ifndef MAIN_GAME_SCENESERIALIZER_H
#define MAIN_GAME_SCENESERIALIZER_H
#include "Scene.h"

class SceneSerializer {
public:
    SceneSerializer(Scene* scene);

    void serialize(const std::string& path) const;
    void deserialize(const std::string& path) const;

private:
    Scene* scene;
};


#endif //MAIN_GAME_SCENESERIALIZER_H
