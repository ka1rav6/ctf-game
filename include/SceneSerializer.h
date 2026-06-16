//
// Created by kairav on 6/16/26.
//

#ifndef MAIN_GAME_SCENESERIALIZER_H
#define MAIN_GAME_SCENESERIALIZER_H


class SceneSerializer {
public:
    SceneSerializer(Scene* scene);

    void serialize(const std::string& path);
    void deserialize(const std::string& path);

private:
    Scene* scene;
};


#endif //MAIN_GAME_SCENESERIALIZER_H
