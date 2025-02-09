/*
-- CameraModule of SceneEngine --

- Handles camera movement and projection matrixes.
- Is treated like a special object by SceneEngine.
*/

#pragma once

#include <vmmlib/vector.hpp>


struct CameraCrate {
    vmml::vec3f position;
    vmml::vec3f direction;
    float fov;
};

struct CameraBufferCrate {
    alignas(16) vmml::vec3f position;
    alignas(16) vmml::vec3f direction;
    float fov;
};

class CameraEngine {
public:
    CameraEngine();
    ~CameraEngine();

    void buildCrate(CameraBufferCrate& crate) const;
    void buildCrate(CameraCrate& crate) const;
    void applyCrate(const CameraCrate& crate);

    void init(const CameraCrate& crate);
    void update();

private:
    vmml::vec3f position;
    vmml::vec3f direction;
    float fov;
};
