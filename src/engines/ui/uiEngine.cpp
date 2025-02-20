#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glm/glm.hpp>
#include <cwctype>

#include <engines/window/windowEngine.hpp>
#include <engines/render/renderEngine.hpp>
#include <engines/scene/sceneEngine.hpp>
#include <engines/camera/cameraEngine.hpp>
#include <engines/ui/uiEngine.hpp>


//////////////////////
////// Builders //////
//////////////////////
UiEngine::UiEngine()  { }
UiEngine::~UiEngine() { }


//////////////////
////// Main //////
//////////////////
void UiEngine::init(const UiUtils::UiCrate& crate, const WindowEngine& windowEngine) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void) io;

    ImGui_ImplGlfw_InitForOpenGL(windowEngine.getWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 450 core");

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui::StyleColorsDark();
}

void UiEngine::update(RenderEngine& renderEngine, const SceneEngine& sceneEngine, CameraEngine& cameraEngine) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::DockSpaceOverViewport(ImGui::GetID(ImGui::GetMainViewport()), ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
    uiLayout(renderEngine, sceneEngine, cameraEngine);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UiEngine::uiLayout(RenderEngine& renderEngine, const SceneEngine& sceneEngine, CameraEngine& cameraEngine) {
    ImGui::Begin("Shader");

    if (ImGui::CollapsingHeader("Shader Program")) { ImGui::Indent();
        RenderUtils::RenderCrate renderCrate; renderEngine.buildCrate(renderCrate);

        for (auto& [name, shader] : renderCrate.shaders) {
            if (ImGui::RadioButton(name, shader.rendering)) {
                renderCrate.shaders[name].rendering = !renderCrate.shaders[name].rendering;
            }
        }

        renderEngine.applyCrate(renderCrate);
    }

    ImGui::End();

    ImGui::Begin("World");
    
    if (ImGui::CollapsingHeader("Camera")) { ImGui::Indent();
        CameraUtils::CameraCrate crate; cameraEngine.buildCrate(crate);

        ImGui::SliderFloat3("Position", &crate.position.x, -10, 10);
        ImGui::SliderFloat3("Target", &crate.target.x, -20, 20);
        ImGui::SliderFloat("Fov", &crate.fov, 30, 120);

        cameraEngine.applyCrate(crate);
    }

    ImGui::End();
};
