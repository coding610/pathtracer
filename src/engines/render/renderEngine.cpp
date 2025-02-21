#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>
#include <format>

#include <engines/window/windowEngine.hpp>
#include <engines/render/renderEngine.hpp>
#include <engines/scene/sceneEngine.hpp>
#include <engines/camera/cameraEngine.hpp>
#include <engines/render/bufferModule.hpp>
#include <engines/render/shaderModule.hpp>
#include <models/object3D.hpp>


///////////////////
////// Utils //////
///////////////////
void RenderUtils::setFullscreenQuad(GLuint& VAO, GLuint& VBO, GLuint& EBO) {
    float vertices[] = { -1.0f, -1.0f, 1.0f, -1.0f, 1.0f,  1.0f, -1.0f,  1.0f };
    GLuint indices[] = { 0, 1, 2, 2, 3, 0 };
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

const char* RenderUtils::mapGlShaderEnum(const GLenum& shader) {
    switch (shader) {
        case GL_VERTEX_SHADER: return "vertex";
        case GL_FRAGMENT_SHADER: return "fragment";
        case GL_COMPUTE_SHADER: return "compute";

        default:
            spdlog::critical("Failed mapping glShaderEnum [4.4.1]"); std::abort();
    }
}


//////////////////////
////// Builders //////
//////////////////////
RenderEngine::RenderEngine() { }
RenderEngine::~RenderEngine() { }


////////////////////
////// Crates //////
////////////////////
void RenderEngine::buildCrate(RenderUtils::RenderCrate& crate) const { crate.shaders = shaders; }
void RenderEngine::applyCrate(const RenderUtils::RenderCrate& crate) {
    ////// This will update only provided keys //////
    for (const auto& [name, shader]: crate.shaders) {
        if (shaders.find(name) != shaders.end()) {
            shaders[name] = shader;
        }
    }
}


//////////////////
////// Main //////
//////////////////
void RenderEngine::init(const RenderUtils::RenderCrate& crate, const SceneEngine& sceneEngine) {
    shaders = crate.shaders;
    shaderOrder = crate.shaderOrder;
    shaderProject = crate.shaderProject;

    spdlog::info("Creating \t fullscreenQuad  [4.1]");
    RenderUtils::setFullscreenQuad(VAO, VBO, EBO);

    spdlog::info("Initializing \t shaderModule \t [4.2]");
    spdlog::info("Initializing \t bufferModule \t [4.3]");

    spdlog::info("Loading \t shaders \t [4.4]");
    for (const auto& [name, shader] : shaders) {
        if (!shader.loaded) continue;

        shaderModule.loadShader(name, std::format("shaders/{}/{}.glsl", shaderProject, RenderUtils::mapGlShaderEnum(shader.shaderType)).c_str(), shader.shaderType);
    }

    ////// Create buffers //////
    spdlog::info("Creating \t buffers \t [4.5]");
    for (const auto& [name, shader] : shaders) { if (shader.loaded == 1 && name == std::string("pathtracer")) {
        bufferModule.createBuffer("camera", GL_UNIFORM_BUFFER, sizeof(CameraUtils::CameraBufferCrate), 0);

        SceneUtils::SceneCrate sceneCrate; sceneEngine.buildCrate(sceneCrate);
        bufferModule.createBuffer("spheres", GL_SHADER_STORAGE_BUFFER, sceneCrate.objects.size() * sizeof(Sphere), 1);
    }}
}

void RenderEngine::update(const WindowEngine& windowEngine, const SceneEngine& sceneEngine, const CameraEngine& cameraEngine) {
    ////// Clear screen //////
    glClear(GL_COLOR_BUFFER_BIT);

    ////// Using Shaders ///////
    for (const auto& name : shaderOrder) {
        RenderUtils::Shader shader = shaders[name];
        if (shader.rendering == false) continue;
        if (!shader.loaded) {
            shaders[name].loaded = 1;
            shaderModule.loadShader(name, std::format("shaders/{}/{}.glsl", name, RenderUtils::mapGlShaderEnum(shader.shaderType)).c_str(), shader.shaderType);
        }

        shaderModule.useShader(name);

        if (name == std::string("pathtracerFragment")) {
            glm::vec2 dim = windowEngine.getDimensions();
            shaderModule.setUniform("pathtracerFragment", "windowWidth", dim.x);
            shaderModule.setUniform("pathtracerFragment", "windowHeight", dim.y);
            shaderModule.setUniform("pathtracerFragment", "aspectRatio", dim.x / dim.y);

            CameraUtils::CameraBufferCrate cameraCrate; cameraEngine.buildCrate(cameraCrate);
            bufferModule.updateBuffer("camera", &cameraCrate, sizeof(CameraUtils::CameraBufferCrate));

            SceneUtils::SceneCrate sceneCrate; sceneEngine.buildCrate(sceneCrate);
            bufferModule.updateBuffer("spheres", sceneCrate.objects.data(), sceneCrate.objects.size() * sizeof(Sphere));
        }
    }

    ////// Draw For Shader(s) To Draw On //////
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
