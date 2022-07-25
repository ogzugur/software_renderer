#pragma once
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
private:
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    unsigned int viewportWidth;
    unsigned int viewportHeight;
    glm::vec3 cameraPos;
    glm::vec3 cameraDirection;
    glm::vec3 cameraUp;

    float cameraFOV;
    float cameraNear;
    float cameraFar;

    void updateCameraViewMatrix();
    glm::vec3 GetViewDir();
    glm::vec3 GetRightVector();

public:
    Camera(){};
    Camera(unsigned int viewportWidth, unsigned int viewportHeight);
    ~Camera();

    void calculateArcBallPosition(float xOffset, float yOffset);
    void calculateCameraZoom(float scrollOffset);
    glm::mat4 getCameraProjectionMatrix();
    glm::mat4 getCameraViewMatrix();
};
