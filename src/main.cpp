#include <iostream>
#include "frame.h"
#include "model.h"
#include "texture.h"
#include "renderer.h"
#include "shader.h"
#include "viewer.h"
#include <stb/stb_image.h>

#define IMG_WIDHT 800
#define IMG_HEIGHT 800

float rotateValue = 0;

Renderer renderer;
Model dummy_model;
Model cube_model;
Viewer *viewer;
Camera camera;
Frame *frame;

void Run()
{
    float flPreviousTime = 0;
    float flCurrentTime = (float)glfwGetTime();

    while (!glfwWindowShouldClose(viewer->getWindow()))
    {
        viewer->isMouseMoving = false;
        glfwPollEvents();
        flPreviousTime = flCurrentTime;
        flCurrentTime = (float)glfwGetTime();
        float dt = flCurrentTime - flPreviousTime;
        if (viewer->leftMousePressed && viewer->isMouseMoving)
        {
            camera.calculateArcBallPosition(viewer->xoffset, viewer->yoffset);
        }


        rotateValue = rotateValue + (50 * dt);
        glClear(GL_COLOR_BUFFER_BIT);
        frame->clearDepthColor();
        renderer.drawModel(dummy_model);

        cube_model.modelMatrix = glm::mat4(1.0f);
        cube_model.modelMatrix = glm::rotate(cube_model.modelMatrix, glm::radians(rotateValue), glm::vec3(0.0f, 1.0f, 0.0f));
        cube_model.modelMatrix = glm::translate(cube_model.modelMatrix, glm::vec3(-1.5, 0.0, 0.0));
        cube_model.modelMatrix = glm::scale(cube_model.modelMatrix, glm::vec3(0.4));
        renderer.drawModel(cube_model);
        viewer->drawFulScreenQuad(frame->color_buffer);
        glfwSwapBuffers(viewer->getWindow());
    }
    glfwTerminate();
}

int main()
{
    Shader mainShader = Shader();
    viewer = new Viewer(IMG_WIDHT, IMG_HEIGHT);
    camera = Camera(IMG_WIDHT, IMG_HEIGHT);
    frame = new Frame(IMG_WIDHT, IMG_HEIGHT);
    renderer = Renderer(camera, *frame);
    renderer.rendererShader = mainShader;
    dummy_model = Model("resources/models/head/head.obj");
    cube_model = Model("resources/models/cube/cube.obj");

    viewer->OpenWindow();
    Run();
    delete frame;
}
