cmake_minimum_required(VERSION 3.20)
project(main_game)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include(FetchContent)
# -------------------------------------------------------------------
# Dependencies
# -------------------------------------------------------------------
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(assimp REQUIRED)
find_package(Boost REQUIRED)
find_package(nlohmann_json REQUIRED)
# -------------------------------------------------------------------
# ReactPhysics3D
# -------------------------------------------------------------------
FetchContent_Declare(
        reactphysics3d
        GIT_REPOSITORY https://github.com/DanielChappuis/reactphysics3d.git
        GIT_TAG v0.10.2
)
FetchContent_MakeAvailable(reactphysics3d)
# -------------------------------------------------------------------
# ImGui
# -------------------------------------------------------------------
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/external/imgui)
set(IMGUI_SOURCES
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)
# -------------------------------------------------------------------
# Game Sources
# -------------------------------------------------------------------
# src/               -> Game.cpp, Scene.cpp, src/components/*.cpp etc
# include/            -> systems live next to their headers (Renderer.cpp,
#                        Editor.cpp, MyImgui.cpp, PhysicsEngine.cpp)
# abstractions/src/  -> glad.c, Shader.cpp, Model.cpp, Initializer.cpp
# project root       -> main.cpp
file(GLOB_RECURSE GAME_SOURCES
        CONFIGURE_DEPENDS
        ${CMAKE_SOURCE_DIR}/src/*.cpp
        ${CMAKE_SOURCE_DIR}/include/*.cpp
        ${CMAKE_SOURCE_DIR}/abstractions/src/*.cpp
        ${CMAKE_SOURCE_DIR}/abstractions/src/*.c
)

file(GLOB ROOT_SOURCES
        CONFIGURE_DEPENDS
        ${CMAKE_SOURCE_DIR}/*.cpp
)
add_executable(main_game
        ${GAME_SOURCES}
        ${ROOT_SOURCES}
        ${IMGUI_SOURCES}
)
# -------------------------------------------------------------------
# Include Directories
# -------------------------------------------------------------------
target_include_directories(main_game PUBLIC
        # Main project headers
        ${CMAKE_SOURCE_DIR}/include
        # Your abstractions library
        ${CMAKE_SOURCE_DIR}/abstractions/include
        # External libraries
        ${CMAKE_SOURCE_DIR}/external
        # ImGui
        ${IMGUI_DIR}
        ${IMGUI_DIR}/backends
)
# -------------------------------------------------------------------
# Link Libraries
# -------------------------------------------------------------------
target_link_libraries(main_game PRIVATE
        OpenGL::GL
        glfw
        assimp::assimp
        reactphysics3d
        Boost::boost
        nlohmann_json::nlohmann_json
)