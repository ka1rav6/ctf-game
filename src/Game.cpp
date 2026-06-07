//
// Created by kairav on 6/7/26.
//

#include "../include/Game.h"



Game::Game(bool mouseCaptured) {
    init();
    if (!this->window)
        LOG_FATAL("Failed to execute Game::init() function. Window not created!");
    if (mouseCaptured) {
        glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    glfwSetCursorPosCallback(window, Game::mouse_callback);
    glfwSetScrollCallback(window, Game::scroll_callback);
    glfwSetFramebufferSizeCallback(window, Game::framebuffer_size_callback);
    LOG_INFO("Set all different callbacks successfully!");

}

void Game::init() {
    this->window = Initializer::init(settings.SCR_WIDTH, settings.SCR_HEIGHT, settings.WINDOW_TITLE);
    glfwSetWindowUserPointer(this->window, this);
    camera = new Camera(settings.camera_position);
    LOG_INFO("Created window and camera userpointer!");

}

Game::~Game() {
    delete camera;
}

void Game::processInput() {
    if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS)
        camera->ProcessKeyboard(FORWARD, timer.deltaTime);
    if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS)
        camera->ProcessKeyboard(BACKWARD, timer.deltaTime);
    if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS)
        camera->ProcessKeyboard(LEFT, timer.deltaTime);
    if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS)
        camera->ProcessKeyboard(RIGHT, timer.deltaTime);
}
void Game::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Game::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
    if (!game) return;

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (game->firstMouse)
    {
        game->settings.lastX = xpos;
        game->settings.lastY = ypos;
        game->firstMouse = false;
    }

    float xoffset = xpos - game->settings.lastX;
    float yoffset = game->settings.lastY - ypos;

    game->settings.lastX = xpos;
    game->settings.lastY = ypos;

    game->camera->ProcessMouseMovement(xoffset, yoffset);
}
void Game::scroll_callback(GLFWwindow* window, double, double yoffset)
{
    Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
    if (!game) return;

    game->camera->ProcessMouseScroll(static_cast<float>(yoffset));
}