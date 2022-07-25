#include "viewer.h"

#ifdef __APPLE__
    #define VIEWER_WIDTH 800
    #define VIEWER_HEIGHT 800
#elif _WIN32
    #define VIEWER_WIDTH 1024
    #define VIEWER_HEIGHT 1024
#endif
Viewer* Viewer::s_pViewer = NULL;

void Viewer::SetMousePos(double x, double y)
{
	if (m_bFirstMouse)
	{
		m_dLastX = x;
		m_dLastY = y;
		m_bFirstMouse = false;
	}

	xoffset = x - m_dLastX;
	yoffset = m_dLastY - y; // reversed since y-coordinates go from bottom to top

	m_dLastX = x;
	m_dLastY = y;
    Get()->copyCamera->calculateArcBallPosition(xoffset, yoffset);
}

void Viewer::MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    Get()->isMouseMoving = true;
	if (Get()->leftMousePressed)
	{
		Get()->SetMousePos(xpos, ypos);
	}
    else 
	{
		Get()->m_bFirstMouse = true;
	}
}

void Viewer::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        Get()->leftMousePressed = true;
    }
         
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        Get()->leftMousePressed = false;
    }
}

void Viewer::MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    Get()->scrollOffset = -(yoffset * 0.15);
    printf("scrollOffset: %f\n", Get()->scrollOffset);
    Get()->copyCamera->calculateCameraZoom(Get()->scrollOffset);
}

Viewer::Viewer()
{
    this->imgWidth = 0;
    this->imgHeight = 0;
}

Viewer::Viewer(GLuint imgWidth, GLuint imgHeight)
{
    s_pViewer = this;
    this->imgWidth = imgWidth;
    this->imgHeight = imgHeight;
}

Viewer::~Viewer()
{
}

GLFWwindow *Viewer::getWindow()
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

    if (!(window = glfwCreateWindow(VIEWER_WIDTH, VIEWER_HEIGHT, "Window 1", NULL, NULL)))
    {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetScrollCallback(window, MouseScrollCallback);
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

    fullScreenQuadShader = new CShader("../resources/shaders/fullScreenQuadShader.vert", "../resources/shaders/fullScreenQuadShader.frag", "quadShader");
    createTexture(NULL);

    return true;
}

void Viewer::createTexture(void *dataPtr)
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
void Viewer::drawFulScreenQuad(void *frameData)
{
    fullScreenQuadShader->use();
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, imgWidth, imgHeight, GL_RGB, GL_UNSIGNED_BYTE, (unsigned char *)frameData);
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

void Viewer::drawDepthBuffer(void *depthData)
{
    fullScreenQuadShader->use();
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, imgWidth, imgHeight, GL_RED, GL_FLOAT, (float *)depthData);
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
