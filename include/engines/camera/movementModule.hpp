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


class RenderEngine; // Forward Declaration

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
    void update(const WindowEngine& windowEngine, RenderEngine& renderEngine, glm::vec3& direction);

private:
    void setDeltaMouse(const WindowEngine& windowEngine, glm::vec2& deltaMouse);
    void setCameraDirection(const glm::vec2& deltaMouse, glm::vec3& direction);
    void toggleFocus(const WindowEngine& windowEngine, RenderEngine& renderEngine);

    float yaw = 0;
    float pitch = 0;
    float sensitivity;

    bool focused = true;
    bool escapePressed = false;
};

#include <engines/render/renderEngine.hpp> // Forward Declaration
