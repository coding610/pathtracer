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


//////////////////////
////// Builders //////
//////////////////////
RenderEngine::RenderEngine() { }
RenderEngine::~RenderEngine() { }


////////////////////
////// Crates //////
////////////////////
void RenderEngine::buildCrate(RenderCrate& crate) const { crate.shaderStatuses = shaderStatuses; }
void RenderEngine::applyCrate(const RenderCrate& crate) {
    ////// This will update only provided keys //////
    for (const auto& [key, value] : crate.shaderStatuses) {
        if (shaderStatuses.find(key) != shaderStatuses.end()) {
            shaderStatuses[key] = value;
        }
    }
}


//////////////////
////// Main //////
//////////////////
void RenderEngine::init(const RenderCrate& crate, const SceneEngine& sceneEngine) {
    shaderStatuses = crate.shaderStatuses;

    spdlog::info("Creating \t fullscreenQuad  [4.1]");
    RenderUtils::setFullscreenQuad(VAO, VBO, EBO);

    spdlog::info("Initializing \t shaderModule \t [4.2]");
    spdlog::info("Initializing \t bufferModule \t [4.3]");

    spdlog::info("Loading \t shaders \t [4.4]");
    for (const auto& pair : shaderStatuses) {
        if (!pair.second.first) continue;

        shaderModule.loadShader(pair.first, std::format("shaders/{}/vertex.glsl", pair.first).c_str(), std::format("shaders/{}/fragment.glsl", pair.first).c_str());
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
    ////// Clear screen //////
    glClear(GL_COLOR_BUFFER_BIT);


    ////// Using Shaders ///////
    for (const auto& pair : shaderStatuses) { if (pair.second.first == 1) {
        if (!pair.second.second) {
            shaderStatuses[pair.first].second = 1;
            shaderModule.loadShader(pair.first, std::format("shaders/{}/vertex.glsl", pair.first).c_str(), std::format("shaders/{}/fragment.glsl", pair.first).c_str());
        }

        const char* shaderInUse = pair.first;

        if (shaderInUse == std::string("pathtracer")) {
            shaderModule.useShader(shaderInUse);
            glm::vec2 dim = windowEngine.getDimensions();
            shaderModule.setUniform("pathtracer", "windowWidth", dim.x);
            shaderModule.setUniform("pathtracer", "windowHeight", dim.y);
            shaderModule.setUniform("pathtracer", "aspectRatio", dim.x / dim.y);

            CameraBufferCrate cameraCrate; cameraEngine.buildCrate(cameraCrate);
            bufferModule.updateBuffer("camera", &cameraCrate, sizeof(CameraBufferCrate));

            SceneCrate sceneCrate; sceneEngine.buildCrate(sceneCrate);
            bufferModule.updateBuffer("spheres", sceneCrate.objects.data(), sceneCrate.objects.size() * sizeof(Sphere));
        } else if (shaderInUse == std::string("dim")) {
            glBindFramebuffer(GL_FRAMEBUFFER, dimFBO);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, dimTexture, 0);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            shaderModule.useShader(shaderInUse);

            glm::vec2 dim = windowEngine.getDimensions();
            shaderModule.setUniform("dim", "windowWidth", dim.x);
            shaderModule.setUniform("dim", "windowHeight", dim.y);
            shaderModule.setUniform("dim", "dimFactor", 0.5);
        }

        ////// Draw For Shader(s) To Draw On //////
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }}

}
