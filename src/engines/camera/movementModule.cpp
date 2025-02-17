#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>

#include <engines/window/windowEngine.hpp>
#include <engines/render/renderEngine.hpp>
#include <engines/camera/movementModule.hpp>



MovementModule::MovementModule() { }
MovementModule::~MovementModule() { }

void MovementModule::buildCrate(MovementCrate& crate) { crate = { sensitivity }; }
void MovementModule::setCrate(const MovementCrate& crate) { sensitivity = crate.sensitivity; }

void MovementModule::init(const MovementCrate& crate) { setCrate(crate); }
bool MovementModule::update(const WindowEngine& windowEngine, RenderEngine& renderEngine, glm::vec3& direction) {
    glm::vec2 deltaMouse; setDeltaMouse(windowEngine, deltaMouse);
    setCameraDirection(deltaMouse, direction);
    toggleFocus(windowEngine, renderEngine);

    return focused;
}

void MovementModule::setDeltaMouse(const WindowEngine& windowEngine, glm::vec2& deltaMouse) {
    glm::vec2 midPoint = { windowEngine.getDimensions().x / 2, windowEngine.getDimensions().y / 2 };

    double x, y; glfwGetCursorPos(windowEngine.getWindow(), &x, &y);
    deltaMouse = glm::vec2(x, y) - midPoint;

    ////// Prevent wayland mouse hopping //////
    if (deltaMouse.x > 5000 || deltaMouse.x < -5000) deltaMouse.x = 0;
    if (deltaMouse.y > 5000 || deltaMouse.y < -5000) deltaMouse.y = 0;

    ////// Set mouse in the middle //////
    glfwSetCursorPos(windowEngine.getWindow(), midPoint.x, midPoint.y);
}

void MovementModule::setCameraDirection(const glm::vec2& deltaMouse, glm::vec3& direction) {
    yaw += deltaMouse.x * sensitivity;
    pitch -= deltaMouse.y * sensitivity;
    pitch = std::clamp(pitch, -89.0f, 89.0f);

    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
}

void MovementModule::toggleFocus(const WindowEngine& windowEngine, RenderEngine& renderEngine) {
    int state = glfwGetKey(windowEngine.getWindow(), GLFW_KEY_ESCAPE);
    if (state == GLFW_PRESS && !escapePressed) {
        escapePressed = true;
        focused = !focused;

        if (focused) {
            glfwSetInputMode(windowEngine.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

            RenderCrate crate;
            renderEngine.buildCrate(crate);
            crate.shaderStatuses = {{"dim", {0, 1}}};
            renderEngine.applyCrate(crate);
        } else {
            glfwSetInputMode(windowEngine.getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

            RenderCrate crate;
            renderEngine.buildCrate(crate);
            crate.shaderStatuses = {{"dim", {1, 1}}};
            renderEngine.applyCrate(crate);
        }
    } else if (state == GLFW_RELEASE) {
        escapePressed = false;
    }
}
