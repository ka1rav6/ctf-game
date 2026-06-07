//
// Created by kairav on 6/7/26.
//

#ifndef MAIN_GAME_GAME_H
#define MAIN_GAME_GAME_H
#include <string>
#include "../include/common.h"

struct Settings {
    // basic screen settings

    const int SCR_WIDTH = 800;
    const int SCR_HEIGHT = 600;
    const char* WINDOW_TITLE = "CTF IIITD";
    const glm::vec3 bgcolor = glm::vec3(0.0f, 0.0f, 0.0f);

    // Camera settings:
    glm::vec3 camera_position = glm::vec3(0.0f, 0.0f, 4.0f);
    float lastX = (float)SCR_WIDTH / 2.0f;
    float lastY = (float)SCR_HEIGHT / 2.0f;
};
struct Timer {
    float deltaTime = 0.0f;	// time between current frame and last frame
    float lastFrame = 0.0f;
};

class Game {
public:
    Game(bool mouseCaptured);
    Game(Settings settings);
    ~Game();
    void run();
    void cleanup();
    void draw();
private:
    void processInput();
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    bool firstMouse = true;
    void init();
    Settings settings;
    GLFWwindow *window;
    Camera *camera;
    Timer timer;
};


#endif //MAIN_GAME_GAME_H
