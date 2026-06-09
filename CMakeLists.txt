cmake_minimum_required(VERSION 3.20)

project(main_game)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(assimp REQUIRED)

# =========================
# Abstractions library
# =========================

file(GLOB_RECURSE ABSTRACTION_SRCS
        abstractions/src/*.cpp
        abstractions/src/*.c
)

add_library(abstractions STATIC ${ABSTRACTION_SRCS}
        include/Entity_Components.h)

target_include_directories(abstractions
        PUBLIC
        ${CMAKE_SOURCE_DIR}/abstractions/include
        ${CMAKE_SOURCE_DIR}/external
)

target_link_libraries(abstractions
        PUBLIC
        OpenGL::GL
        glfw
        assimp
)

#target_include_directories(main_game PRIVATE external/entt/)
# =========================
# Main Game
# =========================

file(GLOB_RECURSE GAME_SRCS
        src/*.cpp
)

add_executable(main_game
        main.cpp
        ${GAME_SRCS}
        src/Entity.cpp
        include/Entity.h
)

target_include_directories(main_game
        PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(main_game
        PRIVATE
        abstractions
)