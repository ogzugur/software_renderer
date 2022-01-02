#include "viewer.h"

Viewer::Viewer()
{
    this->imgWidth = 0;
    this->imgHeight = 0;
}

Viewer::Viewer(GLuint imgWidth,GLuint imgHeight)
{
    this->imgWidth = imgWidth;
    this->imgHeight = imgHeight;
}

Viewer::~Viewer()
{
}

GLFWwindow* Viewer::getWindow()
{
    return window;
}

bool Viewer::OpenWindow()
{
    int ret = glfwInit();

    if (!ret)
    {
        printf("glfwInit failed\n");
        exit(1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    if (!(window = glfwCreateWindow(1024, 1024, "Window 1", NULL, NULL)))
    {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(false);

    glfwSetTime(0.0);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return -1;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;
    std::cout << glGetString(GL_VENDOR) << std::endl;
    std::cout << glGetString(GL_RENDERER) << std::endl;
    std::cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    fullScreenQuadShader = new CShader("resources/shaders/fullScreenQuadShader.vert", "resources/shaders/fullScreenQuadShader.frag", "quadShader");
    createTexture(NULL);

    return true;
}

void Viewer::createTexture(unsigned char *dataPtr)
{
    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, dataPtr);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
}
void Viewer::drawFulScreenQuad(unsigned char *frameData)
{
    fullScreenQuadShader->use();
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexSubImage2D(GL_TEXTURE_2D, 0,0,0, imgWidth, imgHeight, GL_RGB,GL_UNSIGNED_BYTE, frameData);
    glActiveTexture(GL_TEXTURE0);
    fullScreenQuadShader->SetUniform("frameTexture", 0);
    static GLuint gridVAO;
    if (!gridVAO)
    {
        glGenVertexArrays(1, &gridVAO);
    }

    glBindVertexArray(gridVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
