//
// Created by kairav on 6/5/26.
//

#ifndef MAIN_GAME_SRC_COMMON_H
#define MAIN_GAME_SRC_COMMON_H
///// COMMON FILE THAT INCLUDES ALL PACKAGES AND LIBRARIES/ MODULES

#include "../abstractions/package.h"

#include <iostream>
#include <vector>
#include "../external/glad/glad.h"
#include <GLFW/glfw3.h>

#include "../external/glm/glm.hpp"
#include "../external/glm/gtc/matrix_transform.hpp"
#include "../external/glm/gtc/type_ptr.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/material.h>
#include <assimp/postprocess.h>
#include "Logger.h"
#include <reactphysics3d/reactphysics3d.h>

#include "../external/entt/entt.hpp"
#include "../external/imgui/imgui.h"

#include "../external/imgui/backends/imgui_impl_glfw.h"
#include "../external/imgui/backends/imgui_impl_opengl3.h"

#endif //MAIN_GAME_SRC_COMMON_H
