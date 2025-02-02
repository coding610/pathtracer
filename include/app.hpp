/*
-- Application --

- Main application.
- Manages engines and orchestrates the main loop.
*/

#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <engines/window/windowEngine.hpp>
#include <engines/render/renderEngine.hpp>
#include <engines/scene/sceneEngine.hpp>
#include <engines/ui/uiEngine.hpp>


class App {
public:
    App();
    ~App();

    void run();

private:
    WindowEngine    windowEngine;
    RenderEngine    renderEngine;
    SceneEngine     sceneEngine;
    CameraEngine    cameraEngine;
    UiEngine        uiEngine;
};
