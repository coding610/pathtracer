#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <format>

#include <engines/render/renderEngine.hpp>
#include <engines/scene/sceneEngine.hpp>
#include <engines/camera/cameraEngine.hpp>
#include <engines/render/bufferModule.hpp>
#include <engines/render/shaderModule.hpp>
#include <models/object3D.hpp>
#include <debug.hpp>


//////////////////////
////// Builders //////
//////////////////////
RenderEngine::RenderEngine() { }
RenderEngine::~RenderEngine() { }


////////////////////
////// Crates //////
////////////////////
void RenderEngine::buildCrate(RenderCrate& crate) const { crate.shaderStatuses = shaderStatuses; }
void RenderEngine::applyCrate(const RenderCrate& crate) { shaderStatuses = crate.shaderStatuses; }


//////////////////
////// Main //////
//////////////////
void RenderEngine::init(const RenderCrate& crate, const SceneEngine& sceneEngine) {
    shaderStatuses = crate.shaderStatuses;
    ShaderCrate shaderCrate; shaderModule.init(shaderCrate);
    BufferCrate bufferCrate; bufferModule.init(bufferCrate);

    ////// Setup fullscreen quad to draw on //////
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

    ////// Load shaders //////
    for (const auto& pair : shaderStatuses) {
        const char* key = pair.first;
        shaderModule.loadShader(key, std::format("shaders/{}/{}.vert.glsl", key, key).c_str(), std::format("shaders/{}/{}.frag.glsl", key, key).c_str());
    }

    ////// Create buffers //////
    for (const auto& pair : shaderStatuses) { if (pair.second == 1) {
        if (pair.first == std::string("debug")) { }
        else if (pair.first == std::string("wave")) { }
        else if (pair.first == std::string("pathtracer")) {
            bufferModule.createBuffer("camera", GL_UNIFORM_BUFFER, sizeof(CameraBufferCrate), 0);

            SceneCrate sceneCrate; sceneEngine.buildCrate(sceneCrate);
            bufferModule.createBuffer("spheres", GL_SHADER_STORAGE_BUFFER, sizeof(sceneCrate.objects), 1);
        }
    }}
}

void RenderEngine::update(const SceneEngine& sceneEngine, const CameraEngine& cameraEngine) {
    ////// Clear screen //////
    glClear(GL_COLOR_BUFFER_BIT);

    ////// Shaders ///////
    for (const auto& pair : shaderStatuses) { if (pair.second == 1) {
        shaderInUse = pair.first;
        shaderModule.useShader(shaderInUse);
        break;
    }}


    if (shaderInUse == std::string("wave")) {
        shaderModule.setUniform("wave", "time", glfwGetTime());
    } else if (shaderInUse == std::string("pathtracer")) {
        CameraBufferCrate cameraCrate; cameraEngine.buildCrate(cameraCrate);
        bufferModule.updateBuffer("camera", &cameraCrate, sizeof(CameraBufferCrate));
        // bufferModule.updateBuffer("spheres", &sceneEngine.getObjects(), sceneEngine.getObjects().size() * sizeof(Sphere));
    }

    ////// Draw Quad //////
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
