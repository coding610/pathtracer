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
void RenderUtils::setRenderBuffers(GLuint& pathtracerFBO, GLuint& renderedTexture, GLuint& quadVAO, const glm::vec2& dimensions) {
    ////// Create Path Tracer FBO //////
    spdlog::info("Creating \t FBO \t\t [4.1.1]");
    glGenFramebuffers(1, &pathtracerFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, pathtracerFBO);

    glGenTextures(1, &renderedTexture);
    glBindTexture(GL_TEXTURE_2D, renderedTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, dimensions.x, dimensions.y, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderedTexture, 0);

    spdlog::info("Veryfying \t FBO \t\t [4.1.2]");
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) { spdlog::critical("pathtracerTexture failed to initialize [4.1.3.E1]"); std::abort(); }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    ////// Create fullscreen quad //////
    spdlog::info("Creating \t fullscreenQuad  [4.1.3]");
    float quadVertices[] = {
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f,
        -1.0f,  1.0f,  0.0f, 1.0f
    };

    GLuint quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


//////////////////////
////// Builders //////
//////////////////////
RenderEngine::RenderEngine() { }
RenderEngine::~RenderEngine() { }


////////////////////
////// Crates //////
////////////////////
void RenderEngine::buildCrate(RenderCrate& crate) const { crate.shaderStatuses = shaderStatuses; crate.compShaderStatuses = compShaderStatuses; }
void RenderEngine::applyCrate(const RenderCrate& crate) {
    ////// This will update only provided keys //////

    for (const auto& [key, value] : crate.shaderStatuses) {
        if (shaderStatuses.find(key) != shaderStatuses.end()) {
            shaderStatuses[key] = value;
        }
    } for (const auto& [key, value] : crate.compShaderStatuses) {
        if (compShaderStatuses.find(key) != compShaderStatuses.end()) {
            compShaderStatuses[key] = value;
        }
    }
}


//////////////////
////// Main //////
//////////////////
void RenderEngine::init(const RenderCrate& crate, const WindowEngine& windowEngine, const SceneEngine& sceneEngine) {
    shaderStatuses = crate.shaderStatuses;
    compShaderStatuses = crate.compShaderStatuses;

    ////// Creating Buffers //////
    spdlog::info("Initializing \t buffers \t [4.1]");
    RenderUtils::setRenderBuffers(pathtracerFBO, renderedTexture, quadVAO, windowEngine.getDimensions()); 

    spdlog::info("Initializing \t shaderModule \t [4.2]");
    spdlog::info("Initializing \t bufferModule \t [4.3]");

    spdlog::info("Loading \t shaders \t [4.4]");
    spdlog::info("Loading \t f/v-Shaders \t [4.4.1]");
    for (const auto& pair : shaderStatuses) {
        if (!pair.second.second) continue;

        shaderModule.loadShader(pair.first, std::format("shaders/{}/vertex.glsl", pair.first).c_str(), std::format("shaders/{}/fragment.glsl", pair.first).c_str());
    }

    spdlog::info("Loading \t compShaders \t [4.4.2]");
    for (const auto& pair : compShaderStatuses) {
        if (!pair.second.second) continue;

        shaderModule.loadShader(pair.first, std::format("shaders/{}/compute.glsl", pair.first).c_str());
    }

    ////// Create buffers //////
    spdlog::info("Creating \t buffers \t [4.5]");
    for (const auto& pair : shaderStatuses) { if (pair.second.first == 1 && pair.first == std::string("pathtracer")) {
        bufferModule.createBuffer("camera", GL_UNIFORM_BUFFER, sizeof(CameraBufferCrate), 0);

        SceneCrate sceneCrate; sceneEngine.buildCrate(sceneCrate);
        bufferModule.createBuffer("spheres", GL_SHADER_STORAGE_BUFFER, sceneCrate.objects.size() * sizeof(Sphere), 1);
    }}
}

void RenderEngine::update(const WindowEngine& windowEngine, const SceneEngine& sceneEngine, const CameraEngine& cameraEngine) {
    glClear(GL_COLOR_BUFFER_BIT);

    ////// Comp Shaders //////
    for (const auto& pair : compShaderStatuses) { if (pair.second.first == 1) {
        if (!pair.second.second) {
            compShaderStatuses[pair.first].second = 1;
            shaderModule.loadShader(pair.first, std::format("shaders/{}/compute.glsl", pair.first).c_str());
        }

        if (pair.first == std::string("pathtracer")) {
            shaderModule.useShader("pathtracer");
            glBindFramebuffer(GL_FRAMEBUFFER, pathtracerFBO);
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) { spdlog::critical("Failed to bind framebuffer."); std::abort(); }
            glDispatchCompute(windowEngine.getDimensions().x / 16, windowEngine.getDimensions().y / 16, 1);
            glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            glm::vec2 dim = windowEngine.getDimensions();
            shaderModule.setUniform("pathtracer", "windowWidth", dim.x);
            shaderModule.setUniform("pathtracer", "windowHeight", dim.y);
            shaderModule.setUniform("pathtracer", "aspectRatio", dim.x / dim.y);

            CameraBufferCrate cameraCrate; cameraEngine.buildCrate(cameraCrate);
            bufferModule.updateBuffer("camera", &cameraCrate, sizeof(CameraBufferCrate));

            SceneCrate sceneCrate; sceneEngine.buildCrate(sceneCrate);
            bufferModule.updateBuffer("spheres", sceneCrate.objects.data(), sceneCrate.objects.size() * sizeof(Sphere));

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
    }}

    ////// Frag Shaders //////
    for (const auto& pair : shaderStatuses) { if (pair.second.first == 1) {
        if (!pair.second.second) {
            shaderStatuses[pair.first].second = 1;
            shaderModule.loadShader(pair.first, std::format("shaders/{}/vertex.glsl", pair.first).c_str(), std::format("shaders/{}/fragment.glsl", pair.first).c_str());
        }

        if (pair.first == std::string("dim")) {
            shaderModule.useShader("dim");
        } else {
            shaderModule.useShader(0);
        }
    }}

    ////// Display Rendered Texture //////
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderedTexture);
    if (glGetError() != GL_NO_ERROR) { spdlog::critical("Failed to bind texture."); std::abort(); }

    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
