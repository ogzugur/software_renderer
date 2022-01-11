#include <iostream>
#include "frame.h"
#include "model.h"
#include "texture.h"
#include "renderer.h"
#include "shader.h"
#include "viewer.h"
#include <stb/stb_image.h>

#define IMG_WIDHT 1024
#define IMG_HEIGHT 1024

Renderer renderer;
Model dummy_model;
Viewer viewer;
Camera camera;
Frame *frame;

void Run()
{
    while (!glfwWindowShouldClose(viewer.getWindow()))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        frame->clearDepthColor();
        renderer.drawModel(dummy_model);
        viewer.drawFulScreenQuad(frame->color_buffer);
        glfwPollEvents();
        glfwSwapBuffers(viewer.getWindow());
    }
    glfwTerminate();
}

int main()
{
    Shader mainShader = Shader();
    viewer = Viewer(IMG_WIDHT, IMG_HEIGHT);
    camera = Camera(IMG_WIDHT, IMG_HEIGHT);
    frame = new Frame(IMG_WIDHT, IMG_HEIGHT);
    renderer = Renderer(camera, *frame);
    renderer.rendererShader = mainShader;
    dummy_model = Model("resources/models/cube/cube.obj");

    viewer.OpenWindow();
    Run();
    delete frame;
}
