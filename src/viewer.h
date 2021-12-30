#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "CShader.h"

class Viewer
{
private:
    GLFWwindow *window;
    CShader *fullScreenQuadShader;
    GLuint textureID;
    GLuint imgWidth, imgHeight;

    void createTexture(unsigned char *dataPtr);
public:
    Viewer();
    Viewer(GLuint imgWidth,GLuint imgHeight);
    ~Viewer();
    
    void drawFulScreenQuad(unsigned char *frameData);
    GLFWwindow* getWindow();
    bool OpenWindow();
};
