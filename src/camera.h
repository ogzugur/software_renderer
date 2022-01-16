#pragma once
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
private:
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::mat4 viewportMatrix;

    unsigned int viewportWidth;
    unsigned int viewportHeight;
    glm::vec3 cameraPos;
    glm::vec3 cameraDirection;
    glm::vec3 cameraUp;

    float cameraFOV;
    float cameraNear;
    float cameraFar;

public:
    Camera(){};
    Camera(unsigned int viewportWidth, unsigned int viewportHeight);
    ~Camera();

    void setCameraViewport(unsigned int w, unsigned int h);

    glm::mat4 getCameraProjectionMatrix();
    glm::mat4 getCameraViewMatrix();
    glm::mat4 getCameraViewportMatrix();
};
