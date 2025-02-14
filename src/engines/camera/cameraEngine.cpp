#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <spdlog/spdlog.h>

#include <engines/window/windowEngine.hpp>
#include <engines/render/renderEngine.hpp>
#include <engines/camera/movementModule.hpp>
#include <engines/camera/cameraEngine.hpp>

///////////////////
////// Utils //////
///////////////////
void CameraUtils::calculateProjections(
    const CameraCrate& crate,
    const glm::vec2& dimensions,
    glm::mat4x4& projection,
    glm::mat4x4& view,
    glm::mat4x4& viewProjection,
    glm::mat4x4& inverseViewProjection
) {
    projection = glm::perspective(glm::radians(crate.fov), dimensions.x / dimensions.y, 0.1f, 100.0f);
    view = glm::lookAt(crate.position, crate.target, glm::vec3(0, 1, 0));
    viewProjection = projection * view;
    inverseViewProjection = glm::inverse(viewProjection);
}


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
    applyCrate(crate);

    spdlog::info("Initializing \t MovementModule  [2.1]");
    MovementCrate movementCrate;
    movementCrate.sensitivity = 0.5;
    movementModule.init(movementCrate);

    spdlog::info("Initializing \t Projections \t [2.2]");
    CameraUtils::calculateProjections(
        {position, target, fov},
        windowEngine.getDimensions(),
        projection,
        view,
        viewProjection,
        inverseViewProjection
    );
}

void CameraEngine::update(const WindowEngine& windowEngine, RenderEngine& renderEngine) {
    glm::vec3 direction;
    if (movementModule.update(windowEngine, renderEngine, direction)) { // Returns true when focused
        target = position + direction;
    }

    CameraUtils::calculateProjections(
        {position, target, fov},
        windowEngine.getDimensions(),
        projection,
        view,
        viewProjection,
        inverseViewProjection
    );
}
