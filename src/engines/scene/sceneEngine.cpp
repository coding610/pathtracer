#include <GLFW/glfw3.h>

#include <engines/scene/sceneEngine.hpp>
#include <models/object3D.hpp>
#include <vmmlib/vector.hpp>


//////////////////////
////// Builders //////
//////////////////////
SceneEngine::SceneEngine() : objects({}) { }
SceneEngine::~SceneEngine() {}


////////////////////
////// Crates //////
////////////////////
void SceneEngine::buildCrate(SceneCrate& crate) const { crate.objects = objects; }
void SceneEngine::applyCrate(const SceneCrate& crate) { objects = crate.objects; }


//////////////////
////// Main //////
//////////////////
void SceneEngine::init(const SceneCrate& crate) { }
void SceneEngine::update() {
    glfwPollEvents();
}
