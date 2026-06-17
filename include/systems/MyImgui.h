//
// Created by kairav on 6/17/26.
//

#ifndef MAIN_GAME_MYIMGUI_H
#define MAIN_GAME_MYIMGUI_H

#include "../include/common.h"

class MyImgui {
public:
    static ImGuiIO& init(GLFWwindow*);
    static void beginFrame();
    static void endFrame();
    static void cleanup();
};


#endif //MAIN_GAME_MYIMGUI_H
