cmake_minimum_required(VERSION 3.31)
project(cube)

set(CMAKE_CXX_STANDARD 23)

find_package(glm REQUIRED)

include_directories(include)

add_executable(cube src/main.cpp
        include/cube/core/constants.hpp
        include/cube/world/chunk.hpp
        include/cube/core/block.hpp
        src/world/chunk.cpp
        include/cube/base/generator.hpp
        include/cube/core/types.hpp
        include/cube/world/world.hpp
        src/world/world.cpp
        include/cube/generators/generator_flat.hpp
        src/generators/generator_flat.cpp
        include/cube/utils/utils.hpp
        src/utils/utils.cpp
        include/cube/graphics/shader.hpp
        include/cube/graphics/texture.hpp
        include/cube/graphics/vertex.hpp)

target_include_directories(cube PRIVATE
        ${GLM_INCLUDE_DIRS}
)

target_link_libraries(cube PRIVATE   glm::glm)
