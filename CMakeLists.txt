cmake_minimum_required(VERSION 3.20)

project(main_game LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# =========================
# Dependencies
# =========================
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(assimp REQUIRED)
find_package(nlohmann_json REQUIRED)

include(FetchContent)

FetchContent_Declare(
        reactphysics3d
        GIT_REPOSITORY https://github.com/DanielChappuis/reactphysics3d.git
        GIT_TAG master
)

FetchContent_MakeAvailable(reactphysics3d)

# =========================
# ImGui
# =========================
set(IMGUI_SRC
        external/imgui/imgui.cpp
        external/imgui/imgui_draw.cpp
        external/imgui/imgui_tables.cpp
        external/imgui/imgui_widgets.cpp
        external/imgui/backends/imgui_impl_glfw.cpp
        external/imgui/backends/imgui_impl_opengl3.cpp
)

add_library(imgui STATIC ${IMGUI_SRC})

target_include_directories(imgui
        PUBLIC
        ${CMAKE_SOURCE_DIR}/external/imgui
        ${CMAKE_SOURCE_DIR}/external/imgui/backends
)

target_link_libraries(imgui
        PUBLIC
        glfw
        OpenGL::GL
)

# =========================
# Abstractions (engine layer)
# =========================
file(GLOB_RECURSE ABSTRACTION_SRCS CONFIGURE_DEPENDS
        abstractions/src/*.cpp
        abstractions/src/*.c
)

add_library(abstractions STATIC
        ${ABSTRACTION_SRCS}
        include/systems/Renderer.cpp
        include/systems/PhysicsEngine.cpp
)

target_include_directories(abstractions
        PUBLIC
        ${CMAKE_SOURCE_DIR}/abstractions/include
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/external
)

target_link_libraries(abstractions
        PUBLIC
        OpenGL::GL
        glfw
        assimp
        reactphysics3d
        imgui
)

# =========================
# Game executable
# =========================
file(GLOB_RECURSE GAME_SRCS CONFIGURE_DEPENDS
        src/*.cpp
)

add_executable(main_game
        main.cpp
        ${GAME_SRCS}
        src/SceneSerializer.cpp
        include/systems/MyImgui.cpp
)

target_include_directories(main_game
        PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/abstractions/include
        ${CMAKE_SOURCE_DIR}/external
)

target_link_libraries(main_game
        PRIVATE
        abstractions
        nlohmann_json::nlohmann_json
)