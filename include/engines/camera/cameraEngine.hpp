/*
-- CameraModule of SceneEngine --

- Handles camera movement and projection matrixes.
- Is treated like a special object by SceneEngine.
*/

#pragma once

#include <glm/glm.hpp>

#include <engines/window/windowEngine.hpp>


struct CameraCrate {
    glm::vec3 position;
    glm::vec3 target;
    float fov;
};

struct CameraBufferCrate {
    alignas(16) glm::vec3 position;
    alignas(16) glm::mat4x4 inverseViewProjection;
};

class CameraEngine {
public:
    CameraEngine();
    ~CameraEngine();

    void buildCrate(CameraBufferCrate& crate) const;
    void buildCrate(CameraCrate& crate) const;
    void applyCrate(const CameraCrate& crate);

    void init(const CameraCrate& crate, const WindowEngine& windowEngine);
    void update(const WindowEngine& windowEngine);

private:
    glm::vec3 position;
    glm::vec3 target;
    float fov;

    glm::mat4x4 projection;
    glm::mat4x4 view;
    glm::mat4x4 viewProjection;
    glm::mat4x4 inverseViewProjection;
};
