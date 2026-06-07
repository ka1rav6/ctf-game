cmake_minimum_required(VERSION 3.20)

project(main_game)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)
find_package(assimp REQUIRED)

# =========================
# OpenGL abstraction library
# =========================

file(GLOB ABSTRACTION_SRCS
        abstractions/src/*.cpp
        abstractions/src/*.c
)

add_library(abstractions STATIC
        ${ABSTRACTION_SRCS}
        abstractions/include/Mesh.h
        abstractions/src/Mesh.cpp
        abstractions/src/Model.cpp
        abstractions/include/Model.h
)

target_include_directories(abstractions PUBLIC
        abstractions/include
)

target_link_libraries(abstractions PUBLIC
        assimp
)

# =========================
# Game core library
# =========================

add_library(game_core STATIC
        src/gameState.cpp
        src/Player.cpp
        src/Point.cpp
)

target_include_directories(game_core PUBLIC
        include
)

target_link_libraries(game_core PUBLIC
        abstractions
)

# =========================
# Server
# =========================

add_executable(server
        src/server_main.cpp
)

target_link_libraries(server PRIVATE
        game_core
        Threads::Threads
)

# =========================
# Client
# =========================

add_executable(client
        src/client_main.cpp
)

target_link_libraries(client PRIVATE
        game_core
        Threads::Threads
)