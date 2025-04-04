#include "Window.h"

// Initialize static members
unsigned int Window::m_width = 0;
unsigned int Window::m_height = 0;
const char* Window::m_title = nullptr;
GLFWwindow* Window::m_window = nullptr;
float Window::beginTime = 0.0f;
float Window::endTime = 0.0f;
float Window::dt = 0.0f;
float Window::r = 0.0f;
float Window::g = 0.0f;
float Window::b = 0.0f;
float Window::a = 1.0f;

bool Window::init(unsigned int width, unsigned int height, const char* title) {
    //Initialize variables:
    m_width = width;
    m_height = height;
    m_title = title;
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // Create the GLFW window
    m_window = glfwCreateWindow(m_width, m_height, m_title, nullptr, nullptr);
    if (!m_window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_window);
    //Enable v-sync { Locks the FPS to the monitor refreshing rate! }
    glfwSwapInterval(1);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        std::cout << "Failed to initialize glew" << std::endl;
        glfwTerminate();
        return false;
    }

    // Enable OpenGL features
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set viewport and resize callback
    glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);
    //Setup of MouseListener:
    glfwSetCursorPosCallback(m_window, MouseListener::mousePosCallback);
    glfwSetMouseButtonCallback(m_window, MouseListener::mouseButtonCallback);
    glfwSetScrollCallback(m_window, MouseListener::mouseScrollCallback);
    //Setup of KeyListener:
    glfwSetKeyCallback(m_window, KeyListener::keyCallback);

    // Set initial viewport
    glViewport(0, 0, width, height);

    //Delta Time Initialization:
    beginTime = Time::getTime();
    endTime;
    dt = -1.0f;

    return true;
}

void Window::processInput() {
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(m_window, true);
    }
}

bool Window::shouldClose() {
    return glfwWindowShouldClose(m_window);
}

void Window::update() {

    endTime = Time::getTime();
    dt = endTime - beginTime;
    beginTime = endTime;

    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void Window::clearScreen()
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

GLFWwindow* Window::getGLFWWindow()
{
    return m_window;
}


void Window::getFPS()
{
    std::cout << " " << (1.0f / dt) << "FPS" << std::endl;
}

void Window::cleanup()
{
    glfwTerminate();
}

void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    m_width = width;
    m_height = height;
    glViewport(0, 0, width, height);
}


