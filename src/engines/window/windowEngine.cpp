#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <stdexcept>

#include <engines/window/windowEngine.hpp>
#include <debug.hpp>


//////////////////////
////// Builders //////
//////////////////////
WindowEngine::WindowEngine() { }

WindowEngine::~WindowEngine() {
    glfwDestroyWindow(window);
    glfwTerminate();
};


/////////////////////////////////
////// Setters and Getters //////
/////////////////////////////////
GLFWwindow*           WindowEngine::getWindow()      const { return window; }
const glm::vec2&    WindowEngine::getDimensions()  const { return dimensions;  }
const char*           WindowEngine::getTitle()       const { return title; }


//////////////////
////// Main //////
//////////////////
void WindowEngine::init(const WindowCrate& crate) {
    ////// Members //////
    dimensions = crate.dimensions;
    title      = crate.title;

    ////// Glfw //////
    if (!glfwInit()) { throw std::runtime_error("Fatal Error: App::WindowEngine::init()::glfwInit()"); }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(dimensions.x, dimensions.y, title, nullptr, nullptr);
    if (!window) { throw std::runtime_error("Fatal Error: App::WindowEngine::init()::glfwCreateWindow()"); }

    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);

    ////// GLEW //////
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) { throw std::runtime_error("Fatal Error: App::WindowEngine::init()::glewInit()"); }
    glViewport(0, 0, dimensions.x, dimensions.y);
    glDisable(GL_DEPTH_TEST);
}

void WindowEngine::update() {
    ////// Update res //////
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    dimensions = {static_cast<float>(width), static_cast<float>(height)};

    glfwSwapBuffers(window);
}
