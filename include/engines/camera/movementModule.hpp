/*
-- MovementModule of CameraEngine of App --

- Handles camera mouse and keyboard movement
- Does not manage view and projection matrixes

*/

#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <engines/window/windowEngine.hpp>


struct MovementCrate {
    float sensitivity;
};

class MovementModule {
public:
    MovementModule();
    ~MovementModule();

    void buildCrate(MovementCrate& crate);
    void setCrate(const MovementCrate& crate);

    void init(const MovementCrate& crate);
    void update(const WindowEngine& windowEngine, glm::vec3& direction);

private:
    void setDeltaMouse(const WindowEngine& windowEngine, glm::vec2& deltaMouse);
    void setCameraDirection(const glm::vec2& deltaMouse, glm::vec3& direction);
    void toggleFocus(const WindowEngine& windowEngine);

    glm::vec2 lastMouse;
    float yaw = 0;
    float pitch = 0;
    float sensitivity;

    bool focused = 1;
    bool escapePressed = 0;
};
