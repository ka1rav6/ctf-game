cmake_minimum_required(VERSION 3.20)

project(main_game)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# =========================
# Dependencies
# =========================

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Threads REQUIRED)
find_package(assimp REQUIRED)

# =========================
# GLM (header-only)
# =========================

add_library(glm INTERFACE)
target_include_directories(glm INTERFACE
        ${CMAKE_SOURCE_DIR}/external
)

# =========================
# Abstractions Library
# =========================

file(GLOB ABSTRACTION_SRCS
        abstractions/src/*.cpp
        abstractions/src/*.c
)

add_library(abstractions STATIC ${ABSTRACTION_SRCS})

target_include_directories(abstractions
        PUBLIC
        ${CMAKE_SOURCE_DIR}/abstractions/include
        ${CMAKE_SOURCE_DIR}/external
)

target_link_libraries(abstractions
        PUBLIC
        glfw
        OpenGL::GL
        assimp
        glm
)

# =========================
# Game Sources
# =========================

file(GLOB_RECURSE GAME_SRCS
        src/*.cpp
)

add_library(game_core STATIC ${GAME_SRCS}
        src/components/InfiniteGrid.cpp
        src/components/InfiniteGrid.h)

target_include_directories(game_core
        PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(game_core
        PUBLIC
        abstractions
        Threads::Threads
)

# =========================
# Main Executable
# =========================

add_executable(main_game
        main.cpp
)

target_link_libraries(main_game
        PRIVATE
        game_core
)
