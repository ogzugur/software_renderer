cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(TINY_RENDERER)

include_directories(dependencies)
add_executable(TinyRenderer src/main.cpp
                            src/frame.cpp
                            src/model.cpp
                            src/renderer.cpp
                            src/texture.cpp
                            dependencies/stb/stb_image.cpp
                            dependencies/stb/stb_image_write.cpp
                            dependencies/tiny_obj_loader/tiny_obj_loader.cpp)