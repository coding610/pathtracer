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
    WindowCrate windowCrate;
    windowCrate.dimensions = {1920, 1080};
    windowCrate.title = "pathtracer";
    windowEngine.init(windowCrate);

    spdlog::info("Initializing \t cameraEngine \t [2]");
    CameraCrate cameraCrate;
    cameraCrate.position = {0, 0, 0};
    cameraCrate.target   = {0, 0, -1};
    cameraCrate.fov      = 90;
    cameraEngine.init(cameraCrate, windowEngine);

    spdlog::info("Initializing \t sceneEngine \t [3]");
    SceneCrate sceneCrate;
    sceneCrate.objects = {{ .position = {0, 0, -1}, .radius = 2, .material = {{0.5, 1, 0}} }};
    sceneEngine.init(sceneCrate);

    spdlog::info("Initializing \t renderEngine \t [4]");
    RenderCrate renderCrate;
    renderCrate.shaderStatuses = {{"dim", {0, 1}}, {"pathtracer", {1, 1}}};
    renderEngine.init(renderCrate, sceneEngine);

    spdlog::info("Initializing \t uiEngine \t [5]");
    UiCrate uiCrate;
    uiEngine.init(uiCrate, windowEngine);
}; App::~App() { }

//////////////////
////// Main //////
//////////////////
void App::run() {
    while (!glfwWindowShouldClose(windowEngine.getWindow())) {
        sceneEngine.update();
        cameraEngine.update(windowEngine);
        renderEngine.update(windowEngine, sceneEngine, cameraEngine);
        uiEngine.update(renderEngine, sceneEngine, cameraEngine);
        windowEngine.update();
    }
}
