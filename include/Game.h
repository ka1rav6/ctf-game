//
// Created by kairav on 6/7/26.
//

#ifndef MAIN_GAME_GAME_H
#define MAIN_GAME_GAME_H
#include <string>
#include <GLFW/glfw3.h>

#include "../include/common.h"

struct Settings {
    const int SCR_WIDTH = 800;
    const int SCR_HEIGHT = 600;
    const std::string WINDOW_TITLE = "CTF IIITD";
};

class Game {
public:
    Game();
    Game(Settings settings);
    ~Game();
    void run();
    void cleanup();
    void draw();
    void processInput();
private:
    void init();
    Settings settings;
    GLFWwindow *window;
};


#endif //MAIN_GAME_GAME_H
