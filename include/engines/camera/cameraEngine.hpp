/*
-- CameraModule of SceneEngine --

- Handles camera movement and projection matrixes.
- Is treated like a special object by SceneEngine.
*/

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <engines/window/windowEngine.hpp>
#include <engines/camera/movementModule.hpp>


class RenderEngine; // Forward Declaration

namespace CameraUtils {

struct CameraCrate {
    glm::vec3 position;
    glm::vec3 target;
    float fov;
};

struct CameraBufferCrate {
    alignas(16) glm::vec3 position;
    alignas(16) glm::mat4x4 inverseViewProjection;
};

void calculateProjections(
    const CameraCrate& crate,
    const glm::vec2& dimensions,
    glm::mat4x4& projection,
    glm::mat4x4& view,
    glm::mat4x4& viewProjection,
    glm::mat4x4& inverseViewProjection
);

} // CameraEngineUtils

class CameraEngine {
public:
    CameraEngine();
    ~CameraEngine();

    void buildCrate(CameraUtils::CameraBufferCrate& crate) const;
    void buildCrate(CameraUtils::CameraCrate& crate) const;
    void applyCrate(const CameraUtils::CameraCrate& crate);

    void init(const CameraUtils::CameraCrate& crate, const WindowEngine& windowEngine);
    void update(const WindowEngine& windowEngine, RenderEngine& renderEngine);

private:
    MovementModule movementModule;

    glm::vec3 position;
    glm::vec3 target;
    float fov;

    glm::mat4x4 projection;
    glm::mat4x4 view;
    glm::mat4x4 viewProjection;
    glm::mat4x4 inverseViewProjection;
};

#include <engines/render/renderEngine.hpp> // Forward Declaration
