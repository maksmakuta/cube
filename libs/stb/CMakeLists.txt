cmake_minimum_required(VERSION 3.20)
project(stb LANGUAGES C)

add_library(stb INTERFACE)
target_include_directories(stb INTERFACE include)
