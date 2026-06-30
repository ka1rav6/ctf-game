//
// Created by kairav on 6/7/26.
//
#include "../include/Game.h"

// game constructor : calls priate init() function
Game::Game(bool mouseCaptured) {
    init();
    LOG_INFO("init function completely executed");
    if (!this->window)
        LOG_FATAL("Failed to execute Game::init() function. Window not created!");
    // IMGUI initialization must happen BEFORE our own callbacks are installed,
    // since GLFW only allows one callback per event type and we chain into
    // ImGui's handlers manually inside our own callbacks below.
    this->io = &MyImgui::init(this->window);
    // Always start in Menu state with visible cursor
    state = GameState::Menu;
    glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetCursorPosCallback(this->window, Game::mouse_callback);
    glfwSetScrollCallback(this->window, Game::scroll_callback);
    glfwSetKeyCallback(this->window, Game::key_callback);
    glfwSetMouseButtonCallback(this->window, Game::mouse_button_callback);
    LOG_INFO("Set all different callbacks successfully!");
    this->editor = new Editor(cubeVAO);
}

// initializes the window and all other member ptrs
void Game::init() {
    this->window = Initializer::init(settings.SCR_WIDTH, settings.SCR_HEIGHT, settings.WINDOW_TITLE);
    if (!this->window) {
        LOG_FATAL("Failed to initialize window!");
    }
    glfwMakeContextCurrent(this->window);
    glfwSetWindowUserPointer(this->window, this);
    glEnable(GL_DEPTH_TEST);

    camera = new Camera(settings.camera_position);
    LOG_INFO("Created window and camera userpointer!");
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    LOG_INFO("About to create Scene with all entities");
    this->scene = new Scene(cubeVAO);
    LOG_INFO("Scene created with all entities");

    this->renderer = new Renderer();
    LOG_INFO("Renderer system created");


}
// destructor to avoid memory leaks
Game::~Game() {
    delete scene;
    scene = nullptr; // again: removing dangling ptrs coz I have ocd
    delete renderer;
    renderer = nullptr;
    delete editor;
    editor = nullptr;
    MyImgui::cleanup();
    delete camera;
    camera = nullptr;
    LOG_WARN("All member pointers deleted and dangling pointers assigned nullptr");
    // glfw closed and cleaned
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
}
// process the input of ECS key and mouse
void Game::processInput() {
    if (state != GameState::Playing) return;
    if (editorMode) return;
    if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS)
        camera->ProcessKeyboard(FORWARD, timer.deltaTime);
    if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS)
        camera->ProcessKeyboard(BACKWARD, timer.deltaTime);
    if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS)
        camera->ProcessKeyboard(LEFT, timer.deltaTime);
    if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS)
        camera->ProcessKeyboard(RIGHT, timer.deltaTime);
}
// forwards mouse button clicks to ImGui (required for any widget interaction)
void Game::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
}
// function for deciding what happens with the mouse movements
void Game::mouse_callback(GLFWwindow* window, double xposIn, double yposIn){
    ImGui_ImplGlfw_CursorPosCallback(window, xposIn, yposIn);
    Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
    if (!game || game->state != GameState::Playing || game->editorMode) return;
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
// function that decides what happens with the scroll movements
void Game::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
    Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
    if (!game || game->state != GameState::Playing || game->editorMode) return;
    game->camera->ProcessMouseScroll(static_cast<float>(yoffset));
}
// flips between gameplay (cursor locked) and editor (cursor free, ImGui usable) mode
void Game::setEditorMode(bool enabled) {
    editorMode = enabled;
    if (editorMode) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    } else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        firstMouse = true; // prevents a camera snap from stale cursor deltas
    }
}
// handles ESC for pause/menu transitions, F1 toggles editor mode
void Game::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
    Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
    if (!game) return;

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        switch (game->state) {
            case GameState::Menu:
            case GameState::GameOver:
                glfwSetWindowShouldClose(window, true);
                break;
            case GameState::Playing:
                game->setState(GameState::Paused);
                break;
            case GameState::Paused:
                game->setState(GameState::Playing);
                break;
        }
    }

    if (key == GLFW_KEY_F1 && action == GLFW_PRESS && game->state == GameState::Playing) {
        game->setEditorMode(!game->editorMode);
    }
}
// the main running loop : renders everything and captures input
void Game::run(){
    // Enable alpha blending globally for the game loop
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // Assimp::Importer importer;
    // const aiScene* scene = importer.ReadFile("../external/models/dibbba.glb", 0);
    // aiVector3D min(1e10f), max(-1e10f);
    // for (unsigned m = 0; m < scene->mNumMeshes; m++) {
    //     auto* mesh = scene->mMeshes[m];
    //     for (unsigned v = 0; v < mesh->mNumVertices; v++) {
    //         auto& p = mesh->mVertices[v];
    //         min.x = std::min(min.x, p.x); max.x = std::max(max.x, p.x);
    //         min.y = std::min(min.y, p.y); max.y = std::max(max.y, p.y);
    //         min.z = std::min(min.z, p.z); max.z = std::max(max.z, p.z);
    //     }
    // }
    // std::cout << "size: " << (max.x-min.x) << " " << (max.y-min.y) << " " << (max.z-min.z) << "\n";
    while(isRunning()){
        float currentFrame = static_cast<float>(glfwGetTime());
        timer.deltaTime = currentFrame - timer.lastFrame;
        timer.lastFrame = currentFrame;

        processInput();

        if (state == GameState::Playing) {
            scene->update(*camera);
        }

        this->draw();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
// the actual drawing function called in the render loop : internally calls renderer to render component wise
void Game::draw() {
    // draw bg color
    glClearColor(
        settings.bgcolor.r,
        settings.bgcolor.g,
        settings.bgcolor.b,
        settings.bgcolor.a
    );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // render scene in all states except Menu
    if (state != GameState::Menu) {
        glm::mat4 projection = glm::perspective( glm::radians(camera->Zoom), (float)settings.SCR_WIDTH / (float)settings.SCR_HEIGHT, 0.1f, 1000.0f );
        renderer->render(*scene, *camera, projection);
    }

    MyImgui::beginFrame();

    switch (state) {
        case GameState::Menu:
            renderMenu();
            break;
        case GameState::Playing:
            editor->render(*scene, editorMode);
            break;
        case GameState::Paused:
            editor->render(*scene, false);
            renderPauseMenu();
            break;
        case GameState::GameOver:
            renderGameOver();
            break;
    }

    MyImgui::endFrame();
}

void Game::setState(GameState newState) {
    state = newState;
    switch (newState) {
        case GameState::Playing:
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            firstMouse = true;
            break;
        default:
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            break;
    }
}

void Game::renderMenu() {
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(
        ImVec2(viewport->WorkPos.x + viewport->WorkSize.x * 0.5f,
               viewport->WorkPos.y + viewport->WorkSize.y * 0.5f),
        ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowBgAlpha(0.85f);

    ImGui::Begin("##MainMenu", nullptr,
        ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings);

    ImGui::SetWindowFontScale(2.5f);
    ImGui::TextColored(ImVec4(0.53f, 0.81f, 0.92f, 1.0f), "CTF IIITD");
    ImGui::SetWindowFontScale(1.2f);
    ImGui::Text("Capture The Flag");
    ImGui::SetWindowFontScale(1.0f);
    ImGui::Dummy(ImVec2(0.0f, 24.0f));

    float bw = 200.0f;
    if (showSettings) {
        ImGui::Text("Settings");
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0.0f, 6.0f));
        ImGui::SliderFloat("Volume", &settingsVolume, 0.0f, 100.0f, "%.0f");
        ImGui::Dummy(ImVec2(0.0f, 12.0f));
        if (ImGui::Button("Back", ImVec2(bw, 0.0f))) {
            showSettings = false;
        }
    } else {
        if (ImGui::Button("Play", ImVec2(bw, 0.0f))) {
            setState(GameState::Playing);
        }
        ImGui::Dummy(ImVec2(0.0f, 6.0f));
        if (ImGui::Button("Settings", ImVec2(bw, 0.0f))) {
            showSettings = true;
        }
        ImGui::Dummy(ImVec2(0.0f, 6.0f));
        if (ImGui::Button("Quit", ImVec2(bw, 0.0f))) {
            glfwSetWindowShouldClose(window, true);
        }
    }

    ImGui::End();
}

void Game::renderPauseMenu() {
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(
        ImVec2(viewport->WorkPos.x + viewport->WorkSize.x * 0.5f,
               viewport->WorkPos.y + viewport->WorkSize.y * 0.5f),
        ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowBgAlpha(0.85f);

    ImGui::Begin("##PauseMenu", nullptr,
        ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings);

    ImGui::SetWindowFontScale(2.0f);
    ImGui::Text("PAUSED");
    ImGui::SetWindowFontScale(1.0f);
    ImGui::Dummy(ImVec2(0.0f, 18.0f));

    float bw = 180.0f;
    if (ImGui::Button("Resume", ImVec2(bw, 0.0f))) {
        setState(GameState::Playing);
    }
    ImGui::Dummy(ImVec2(0.0f, 6.0f));
    if (ImGui::Button("Main Menu", ImVec2(bw, 0.0f))) {
        setState(GameState::Menu);
    }
    ImGui::Dummy(ImVec2(0.0f, 6.0f));
    if (ImGui::Button("Quit", ImVec2(bw, 0.0f))) {
        glfwSetWindowShouldClose(window, true);
    }

    ImGui::End();
}

void Game::renderGameOver() {
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(
        ImVec2(viewport->WorkPos.x + viewport->WorkSize.x * 0.5f,
               viewport->WorkPos.y + viewport->WorkSize.y * 0.5f),
        ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowBgAlpha(0.85f);

    ImGui::Begin("##GameOver", nullptr,
        ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings);

    ImGui::SetWindowFontScale(2.5f);
    ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.2f, 1.0f), "GAME OVER");
    ImGui::SetWindowFontScale(1.0f);
    ImGui::Dummy(ImVec2(0.0f, 20.0f));

    // TODO: Show winner / scores once CTF logic is implemented
    if (ImGui::Button("Main Menu", ImVec2(180.0f, 0.0f))) {
        setState(GameState::Menu);
    }

    ImGui::End();
}