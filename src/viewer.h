#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "CShader.h"
#include "camera.h"

class Viewer
{
private:
    static Viewer *s_pViewer;
    GLFWwindow *window;
    CShader *fullScreenQuadShader;
    GLuint textureID;
    GLuint imgWidth, imgHeight;
    bool m_bFirstMouse;
    double m_dLastX;
    double m_dLastY;



    void createTexture(void *dataPtr);
    void SetMousePos(double x, double y);
    static void MouseCallback(GLFWwindow *window, double xpos, double ypos);
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

public:
    bool leftMousePressed;
    bool isMouseMoving;
    float xoffset;
    float yoffset;
    float scrollOffset = 0;
    Camera* copyCamera;
    static Viewer *Get() { return s_pViewer; };

    Viewer();
    Viewer(GLuint imgWidth, GLuint imgHeight);
    ~Viewer();

    void drawDepthBuffer(void *depthData);
    void drawFulScreenQuad(void *frameData);
    GLFWwindow *getWindow();
    bool OpenWindow();
};
