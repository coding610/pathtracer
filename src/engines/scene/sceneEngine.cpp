#include <GLFW/glfw3.h>

#include <engines/scene/sceneEngine.hpp>
#include <models/object3D.hpp>


//////////////////////
////// Builders //////
//////////////////////
SceneEngine::SceneEngine() : objects({}) { }
SceneEngine::~SceneEngine() {}


////////////////////
////// Crates //////
////////////////////
void SceneEngine::buildCrate(SceneUtils::SceneCrate& crate) const { crate.objects = objects; }
void SceneEngine::applyCrate(const SceneUtils::SceneCrate& crate) { objects = crate.objects; }


//////////////////
////// Main //////
//////////////////
void SceneEngine::init(const SceneUtils::SceneCrate& crate) { applyCrate(crate); }
void SceneEngine::update() {
    glfwPollEvents();
}
