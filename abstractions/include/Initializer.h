//
// Created by kairav on 6/5/26.
//

#ifndef MAIN_GAME_INITIALIZER_H
#define MAIN_GAME_INITIALIZER_H
#include "common.h"


class Initializer {
public:
    static GLFWwindow* init(int width, int height, const char* title);
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};

#endif //MAIN_GAME_INITIALIZER_H
