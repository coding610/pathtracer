#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>

#include <engines/window/windowEngine.hpp>
#include <engines/camera/movementModule.hpp>



MovementModule::MovementModule() { }
MovementModule::~MovementModule() { }

void MovementModule::buildCrate(MovementCrate& crate) { crate = { sensitivity }; }
void MovementModule::setCrate(const MovementCrate& crate) { sensitivity = crate.sensitivity; }

void MovementModule::init(const MovementCrate& crate) { setCrate(crate); }
void MovementModule::update(const WindowEngine& windowEngine, glm::vec3& direction) {
    glm::vec2 deltaMouse; setDeltaMouse(windowEngine, deltaMouse);
    setCameraDirection(deltaMouse, direction);
    toggleFocus(windowEngine);
}

void MovementModule::setDeltaMouse(const WindowEngine& windowEngine, glm::vec2& deltaMouse) {
    double x, y;
    glfwGetCursorPos(windowEngine.getWindow(), &x, &y);
    deltaMouse = glm::vec2(x, y) - lastMouse;
    lastMouse = glm::vec2(x, y);

    ////// Prevent wayland mouse hopping //////
    if (deltaMouse.x > 5000 || deltaMouse.x < -5000) deltaMouse.x = 0;
    if (deltaMouse.y > 5000 || deltaMouse.y < -5000) deltaMouse.y = 0;
}

void MovementModule::setCameraDirection(const glm::vec2& deltaMouse, glm::vec3& direction) {
    yaw += deltaMouse.x * sensitivity;
    pitch -= deltaMouse.y * sensitivity;
    pitch = std::clamp(pitch, -89.0f, 89.0f);

    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
}

void MovementModule::toggleFocus(const WindowEngine& windowEngine) {
    int state = glfwGetKey(windowEngine.getWindow(), GLFW_KEY_ESCAPE);
    if (state == GLFW_PRESS && !escapePressed) { escapePressed = true; focused = !focused;
        if (focused) glfwSetInputMode(windowEngine.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        else glfwSetInputMode(windowEngine.getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    } else if (state == GLFW_RELEASE) { escapePressed = false; }

    if (!focused) return;
}
