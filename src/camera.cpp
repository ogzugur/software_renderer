#include "camera.h"
#define _USE_MATH_DEFINES
#include <math.h>

Camera::Camera(unsigned int viewportWidth, unsigned int viewportHeight)
{
    this->viewportWidth = viewportWidth;
    this->viewportHeight = viewportHeight;

    this->cameraPos = glm::vec3(0.0, 0.0, 4.0);
    this->cameraDirection = glm::vec3(0.0, 0.0, 0.0);
    this->cameraUp = glm::vec3(0.0, 1.0, 0.0);
    this->cameraFOV = 60.0f;
    this->cameraNear = 4.0f;
    this->cameraFar = 20.0f;
}

Camera::~Camera()
{
}

glm::vec3 Camera::GetViewDir(){ return -glm::transpose(viewMatrix)[2]; }
glm::vec3 Camera::GetRightVector(){ return glm::transpose(viewMatrix)[0]; }

void Camera::calculateArcBallPosition(float xOffset, float yOffset)
{
    // Get the homogenous position of the camera and pivot point
    glm::vec4 position(cameraPos.x, cameraPos.y, cameraPos.z, 1);
    glm::vec4 pivot(cameraDirection.x, cameraDirection.y, cameraDirection.z, 1);

    // step 1 : Calculate the amount of rotation given the mouse movement.
    float deltaAngleX = (2 * M_PI / viewportWidth); // a movement from left to right = 2*PI = 360 deg
    float deltaAngleY = (M_PI / viewportHeight);  // a movement from top to bottom = PI = 180 deg
    float xAngle = -xOffset * deltaAngleX * 0.25;
    float yAngle = yOffset * deltaAngleY * 0.25;
    printf("%f\n", yAngle);

    // Extra step to handle the problem when the camera direction is the same as the up vector
    float cosAngle = glm::dot(GetViewDir(), cameraUp);
    if (cosAngle * signbit(deltaAngleY) > 0.99f)
        deltaAngleY = 0;

    // step 2: Rotate the camera around the pivot point on the first axis.
    glm::mat4x4 rotationMatrixX(1.0f);
    rotationMatrixX = glm::rotate(rotationMatrixX, xAngle, cameraUp);
    position = (rotationMatrixX * (position - pivot)) + pivot;

    // step 3: Rotate the camera around the pivot point on the second axis.
    glm::mat4x4 rotationMatrixY(1.0f);
    rotationMatrixY = glm::rotate(rotationMatrixY, yAngle, GetRightVector());
    glm::vec3 finalPosition = (rotationMatrixY * (position - pivot)) + pivot;

    // Update the camera view (we keep the same lookat and the same up vector)
    cameraPos = finalPosition;
}

void Camera::calculateCameraZoom(float scrollOffset)
{
    cameraPos = cameraPos + ((cameraPos - cameraDirection )* scrollOffset);  
}
void Camera::updateCameraViewMatrix()
{
    viewMatrix = glm::lookAt(cameraPos, cameraDirection, cameraUp);
}

glm::mat4 Camera::getCameraProjectionMatrix()
{
    return glm::perspective(glm::radians(cameraFOV), (float)viewportWidth / viewportHeight, cameraNear, cameraFar);
}
glm::mat4 Camera::getCameraViewMatrix()
{
    updateCameraViewMatrix();
    return viewMatrix;
}