//
// Created by kairav on 6/17/26.
//

#ifndef MAIN_GAME_EDITOR_H
#define MAIN_GAME_EDITOR_H
#include "../include/common.h"
#include "../include/Scene.h"

class Editor {
public:
    Editor(unsigned int cubeVAO);
    void render(Scene& scene, bool editorMode);
private:
    unsigned int cubeVAO;
};

#endif //MAIN_GAME_EDITOR_H