cmake_minimum_required(VERSION 4.0)
project(cube LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 3.3 REQUIRED)

add_subdirectory(external/glad)

# Sources
set(SOURCES
        src/main.cpp
        src/cube/window/Window.cpp
        src/cube/window/Window.hpp
        src/cube/utils/Camera.cpp
        src/cube/utils/Camera.hpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE src external/glad/include external/stb)

target_link_libraries(${PROJECT_NAME} PRIVATE glad glfw)
