/*
-- RenderEngine of App --

- Core rendering logic; shader program and rendering pipeline.
- Responsible for initizialsing shaders and buffers (its sub modules).
- Manages two modules, ShaderModule and BufferModule.
*/

#pragma once

#include <GL/glew.h>
#include <unordered_map>

#include <engines/window/windowEngine.hpp>
#include <engines/scene/sceneEngine.hpp>
#include <engines/camera/cameraEngine.hpp>
#include <engines/render/bufferModule.hpp>
#include <engines/render/shaderModule.hpp>


struct RenderCrate {
    std::unordered_map<const char*, std::pair<bool, bool>> shaderStatuses;
};

class RenderEngine {
public:
    RenderEngine();
    ~RenderEngine();

    void init(const RenderCrate& crate, const SceneEngine& sceneEngine);
    void update(const SceneEngine& sceneEngine, const CameraEngine& cameraEngine, const WindowEngine& windowEngine);

    void buildCrate(RenderCrate& crate) const;
    void applyCrate(const RenderCrate& crate);

private:
    ShaderModule shaderModule;
    BufferModule bufferModule;

    std::unordered_map<const char*, std::pair<bool, bool>> shaderStatuses;
    const char* shaderInUse;

    GLuint VAO, VBO, EBO;
};
