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

void setFullscreenQuad(GLuint& VAO, GLuint& VBO, GLuint& EBO);

} // RenderEngineUtils

struct RenderCrate {
    std::unordered_map<const char*, std::pair<bool, bool>> shaderStatuses;
};

class RenderEngine {
public:
    RenderEngine();
    ~RenderEngine();

    void init(const RenderCrate& crate, const SceneEngine& sceneEngine);
    void update(const WindowEngine& windowEngine, const SceneEngine& sceneEngine, const CameraEngine& cameraEngine);

    void buildCrate(RenderCrate& crate) const;
    void applyCrate(const RenderCrate& crate);

private:
    ShaderModule shaderModule;
    BufferModule bufferModule;

    std::unordered_map<const char*, std::pair<bool, bool>> shaderStatuses; // id - { use program, loaded }, in order of render

    GLuint VAO, VBO, EBO;       // Move these to bufferModule
    GLuint dimFBO, dimTexture;  // Move these to bufferModule

};

#include <engines/camera/cameraEngine.hpp> // Forward Declaration
