cmake_minimum_required(VERSION 4.2)
project(cube VERSION 0.1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm CONFIG REQUIRED)

add_subdirectory(libs/glad)

add_executable(${PROJECT_NAME}
        src/main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3 OpenGL::GL glm::glm glad)
target_include_directories(${PROJECT_NAME} PRIVATE include libs/glad/include)
