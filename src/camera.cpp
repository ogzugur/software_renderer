#include "camera.h"

Camera::Camera(unsigned int viewportWidth, unsigned int viewportHeight)
{
    this->viewportWidth = viewportWidth;
    this->viewportHeight = viewportHeight;
    setCameraViewport(this->viewportWidth, this->viewportHeight);

    this->cameraPos = glm::vec3(1.0, 0.5, 0.0);
    this->cameraDirection = glm::vec3(0.0, 0.0, -2.0);
    this->cameraUp = glm::vec3(0.0, 1.0, 0.0);
    this->cameraFOV = 60.0f;
    this->cameraNear = -0.1f;
    this->cameraFar = -10.0f;
}

Camera::~Camera()
{
}

glm::mat4 Camera::getCameraProjectionMatrix()
{
    return glm::perspectiveRH(glm::radians(cameraFOV), (float)viewportWidth / viewportHeight, cameraNear, cameraFar);
}
glm::mat4 Camera::getCameraViewMatrix()
{
    return glm::lookAtRH(cameraPos, cameraDirection, cameraUp);
}
glm::mat4 Camera::getCameraViewportMatrix()
{
    return this->viewportMatrix;
}

void Camera::setCameraViewport(unsigned int w, unsigned int h)
{
    float depth = 255.0f;
    viewportMatrix = glm::mat4(1.0f);
    viewportMatrix[0][3] = w / 2.f;
    viewportMatrix[1][3] = h / 2.f;
    viewportMatrix[2][3] = depth / 2.f;

    viewportMatrix[0][0] = w / 2.f;
    viewportMatrix[1][1] = h / 2.f;
    viewportMatrix[2][2] = depth / 2.f;
}