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

struct Shader {
    bool rendering;
    bool loaded;
    GLenum shaderType;
};

struct RenderCrate {
    const char* shaderProject;
    std::vector<const char*> shaderOrder;
    std::unordered_map<const char*, RenderUtils::Shader> shaders;
};

void setFullscreenQuad(GLuint& VAO, GLuint& VBO, GLuint& EBO);
const char* mapGlShaderEnum(const GLenum& shader);

} // RenderEngineUtils

class RenderEngine {
public:
    RenderEngine();
    ~RenderEngine();

    void init(const RenderUtils::RenderCrate& crate, const SceneEngine& sceneEngine);
    void update(const WindowEngine& windowEngine, const SceneEngine& sceneEngine, const CameraEngine& cameraEngine);

    void buildCrate(RenderUtils::RenderCrate& crate) const;
    void applyCrate(const RenderUtils::RenderCrate& crate);

private:
    ShaderModule shaderModule;
    BufferModule bufferModule;

    const char* shaderProject;
    std::vector<const char*> shaderOrder;
    std::unordered_map<const char*, RenderUtils::Shader> shaders;

    GLuint VAO, VBO, EBO;
};

#include <engines/camera/cameraEngine.hpp> // Forward Declaration
