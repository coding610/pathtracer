/*
-- WindowEngine of App --

- Encapsulates GLFW for window creation and input handling.
- Stores window dimensions, input callbacks and OpenGL contex details.
*/

#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <glm/glm.hpp>


struct WindowCrate {
    glm::vec2 dimensions;
    const char* title;
};

class WindowEngine {
public:
    ////// Default Builders //////
    WindowEngine();
    ~WindowEngine();

    /////// Main ///////
    void init(const WindowCrate& crate);
    void update();

    ////// Getters //////
    [[nodiscard]] GLFWwindow* getWindow() const;
    [[nodiscard]] const glm::vec2& getDimensions() const;
    [[nodiscard]] const char* getTitle() const;

private:
    ////// Glfw //////
    GLFWwindow* window;
    glm::vec2 dimensions;
    const char* title;
};
