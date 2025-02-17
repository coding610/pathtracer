/*
-- RenderEngine of App --

- Core rendering logic; shader program and rendering pipeline.
- Responsible for initizialsing shaders and buffers (its sub modules).
- Manages two modules, ShaderModule and BufferModule.
*/

#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <unordered_map>

#include <engines/window/windowEngine.hpp>
#include <engines/scene/sceneEngine.hpp>
#include <engines/render/bufferModule.hpp>
#include <engines/render/shaderModule.hpp>


class CameraEngine; // Forward Declaration

namespace RenderUtils {

// Todo: Move this to bufferModule
void setRenderBuffers(GLuint& pathtracerFBO, GLuint& renderedTexture, GLuint& quadVAO, const glm::vec2& dimensions);

} // RenderEngineUtils

struct RenderCrate {
    std::unordered_map<const char*, std::pair<bool, bool>> shaderStatuses;
    std::unordered_map<const char*, std::pair<bool, bool>> compShaderStatuses;
};

class RenderEngine {
public:
    RenderEngine();
    ~RenderEngine();

    void init(const RenderCrate& crate, const WindowEngine& windowEngine, const SceneEngine& sceneEngine);
    void update(const WindowEngine& windowEngine, const SceneEngine& sceneEngine, const CameraEngine& cameraEngine);

    void buildCrate(RenderCrate& crate) const;
    void applyCrate(const RenderCrate& crate);

private:
    ShaderModule shaderModule;
    BufferModule bufferModule;

    // id - { use program, loaded }, in order of render
    std::unordered_map<const char*, std::pair<bool, bool>> shaderStatuses;
    std::unordered_map<const char*, std::pair<bool, bool>> compShaderStatuses; // id - { use program, loaded }, in order of render

    GLuint pathtracerFBO, renderedTexture, quadVAO;
};
