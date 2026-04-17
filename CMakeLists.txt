cmake_minimum_required(VERSION 4.2)
project(cube VERSION 0.1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(libs/glad)

find_package(SDL3 REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE
        GLAD
        SDL3::SDL3
        glm::glm
        OpenGL::GL
)

target_include_directories(${PROJECT_NAME} PRIVATE
        include
        libs/glad/include
)