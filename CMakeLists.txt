cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(TINY_RENDERER)

include_directories(dependencies)
add_executable(TinyRenderer src/main.cpp
                            dependencies/stb/stb_image.cpp)