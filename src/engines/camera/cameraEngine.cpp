#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <engines/window/windowEngine.hpp>
#include <engines/camera/cameraEngine.hpp>


//////////////////////
////// Builders //////
//////////////////////
CameraEngine::CameraEngine()  { }
CameraEngine::~CameraEngine() { }


////////////////////
////// Crates //////
////////////////////
void CameraEngine::buildCrate(CameraCrate& crate) const { crate = { position, target, fov }; }
void CameraEngine::buildCrate(CameraBufferCrate& crate) const { crate = { position, inverseViewProjection }; }
void CameraEngine::applyCrate(const CameraCrate& crate) { position = crate.position; target = crate.target; fov = crate.fov; }


//////////////////
////// Main //////
//////////////////
void CameraEngine::init(const CameraCrate& crate, const WindowEngine& windowEngine) {
    position = crate.position;
    target = crate.target;
    fov = crate.fov;

    const glm::vec2 dim = windowEngine.getDimensions();
    const float aspectRatio = dim.x / dim.y;
    projection = glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 100.0f);
    view = glm::lookAt(position, target, glm::vec3(0, 1, 0));
    viewProjection = projection * view;
    inverseViewProjection = glm::inverse(viewProjection);
}

void CameraEngine::update(const WindowEngine& windowEngine) {
    const glm::vec2 dim = windowEngine.getDimensions();
    const float aspectRatio = dim.x / dim.y;
    projection = glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 100.0f);
    view = glm::lookAt(position, target, glm::vec3(0, 1, 0));
    viewProjection = projection * view;
    inverseViewProjection = glm::inverse(viewProjection);
}
