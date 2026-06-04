cmake_minimum_required(VERSION 3.20)
project(main_game)

set(CMAKE_CXX_STANDARD 20)

# -------------------------
# Collect source files automatically
# -------------------------
file(GLOB_RECURSE ENGINE_SOURCES
        ${CMAKE_SOURCE_DIR}/abstractions/src/*.cpp
        ${CMAKE_SOURCE_DIR}/abstractions/src/*.c
)

file(GLOB_RECURSE ENGINE_HEADERS
        ${CMAKE_SOURCE_DIR}/abstractions/include/*.h
        ${CMAKE_SOURCE_DIR}/abstractions/include/*.hpp
)

# -------------------------
# Engine library (my abstractions)
# -------------------------
add_library(engine STATIC
        ${ENGINE_SOURCES}
        ${ENGINE_HEADERS}
        include/common.h
)

target_include_directories(engine PUBLIC
        ${CMAKE_SOURCE_DIR}/abstractions/include
)

# -------------------------
# Main executable
# -------------------------
add_executable(main_game main.cpp)

target_link_libraries(main_game PRIVATE engine)