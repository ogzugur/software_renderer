cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(TINY_RENDERER)

add_executable(TinyRenderer src/main.cpp
                            src/frame.cpp
                            src/model.cpp
                            src/mesh.cpp
                            src/renderer.cpp
                            src/texture.cpp
                            src/camera.cpp
                            src/shader.cpp
                            src/CShader.cpp
                            src/viewer.cpp
                            dependencies/stb/stb_image.cpp
                            dependencies/stb/stb_image_write.cpp
                            dependencies/tiny_obj_loader/tiny_obj_loader.cpp)
                            
add_subdirectory(dependencies/glfw)
add_subdirectory(dependencies/glad)

include_directories(dependencies)
target_link_libraries(TinyRenderer
    PRIVATE
    glfw
    GLAD
)