#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>
#include <stdexcept>

#include <engines/window/windowEngine.hpp>


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
const glm::vec2&      WindowEngine::getDimensions()  const { return dimensions;  }
const char*           WindowEngine::getTitle()       const { return title; }


//////////////////
////// Main //////
//////////////////
void WindowEngine::init(const WindowCrate& crate) {
    ////// Members //////
    dimensions = crate.dimensions;
    title      = crate.title;

    ////// Glfw //////
    spdlog::info("Initializing \t GLFW \t\t [1.1]");
    if (!glfwInit()) { spdlog::critical("GLFW initialization failed [1.1.E1]"); std::abort(); }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    spdlog::info("Initializing \t window \t [1.1.1]");
    window = glfwCreateWindow(dimensions.x, dimensions.y, title, nullptr, nullptr);
    spdlog::info("Verifying \t window \t [1.1.2]");
    if (!window) { spdlog::critical("Window initialization failed [1.1.E2]"); std::abort(); }

    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);

    ////// GLEW //////
    spdlog::info("Initializing \t GLEW \t\t [1.2]");
    glewExperimental = GL_TRUE;

    spdlog::info("Verifying \t GLEW \t\t [1.2.1]");
    if (glewInit() != GLEW_OK) { spdlog::critical("GLEW initialization failed [1.2.E1]"); std::abort(); }

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
