cmake_minimum_required(VERSION 3.31)
project(cube)

set(CMAKE_CXX_STANDARD 23)

include_directories(include)

add_executable(cube src/main.cpp
        include/cube/core/constants.hpp
        include/cube/world/chunk.hpp
        include/cube/core/block.hpp
        src/world/chunk.cpp)

