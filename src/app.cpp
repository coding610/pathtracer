#include <GL/glew.h>
#include <spdlog/spdlog.h>

#include <engines/window/windowEngine.hpp>
#include <engines/scene/sceneEngine.hpp>
#include <engines/render/renderEngine.hpp>
#include <engines/camera/cameraEngine.hpp>
#include <engines/ui/uiEngine.hpp>
#include <models/object3D.hpp>
#include <app.hpp>


//////////////////////
////// Builders //////
//////////////////////
App::App() {
    spdlog::set_level(spdlog::level::debug);

    spdlog::info("Initializing \t windowEngine \t [1]");
    WindowUtils::WindowCrate windowCrate;
    windowCrate.dimensions = {1920, 1080};
    windowCrate.title = "pathtracer";
    windowEngine.init(windowCrate);

    spdlog::info("Initializing \t cameraEngine \t [2]");
    CameraUtils::CameraCrate cameraCrate;
    cameraCrate.position = {0, 0, 0};
    cameraCrate.target   = {0, 0, -1};
    cameraCrate.fov      = 90;
    cameraEngine.init(cameraCrate, windowEngine);

    spdlog::info("Initializing \t sceneEngine \t [3]");
    SceneUtils::SceneCrate sceneCrate;
    sceneCrate.objects = {{ .position = {0, 0, -1}, .radius = 2, .material = {{0.5, 1, 0}} }};
    sceneEngine.init(sceneCrate);

    spdlog::info("Initializing \t renderEngine \t [4]");
    RenderUtils::RenderCrate renderCrate;
    renderCrate.shaderProject = "pathtracer";
    renderCrate.shaderOrder = {"passthroughVertex", "pathtracerFragment"};
    renderCrate.shaders = {
        {"passthroughVertex", {.rendering = 1, .loaded = 1, .shaderType = GL_VERTEX_SHADER}},
        {"pathtracerFragment", {.rendering = 1, .loaded = 1, .shaderType = GL_FRAGMENT_SHADER}}
    };
    renderEngine.init(renderCrate, sceneEngine);

    spdlog::info("Initializing \t uiEngine \t [5]");
    UiUtils::UiCrate uiCrate;
    uiEngine.init(uiCrate, windowEngine);
}; App::~App() { }

//////////////////
////// Main //////
//////////////////
void App::run() {
    while (!glfwWindowShouldClose(windowEngine.getWindow())) {
        sceneEngine.update();
        cameraEngine.update(windowEngine, renderEngine);
        renderEngine.update(windowEngine, sceneEngine, cameraEngine);
        uiEngine.update(renderEngine, sceneEngine, cameraEngine);
        windowEngine.update();
    }
}
