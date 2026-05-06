cmake_minimum_required(VERSION 4.2 FATAL_ERROR)
project(cube VERSION 0.1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenGL REQUIRED)
find_package(SDL3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_executable(${PROJECT_NAME}

    src/main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    OpenGL::GL SDL3::SDL3 glm::glm
)