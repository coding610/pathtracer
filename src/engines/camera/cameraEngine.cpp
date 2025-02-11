#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <spdlog/spdlog.h>

#include <engines/window/windowEngine.hpp>
#include <engines/camera/cameraEngine.hpp>
#include <utils/utils.hpp>


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

    spdlog::info("Initializing \t Projections \t [2.1]");
    utils::setProjections(
        {position, target, fov},
        windowEngine.getDimensions(),
        projection,
        view,
        viewProjection,
        inverseViewProjection
    );
}

void CameraEngine::update(const WindowEngine& windowEngine) {
    utils::setProjections(
        {position, target, fov},
        windowEngine.getDimensions(),
        projection,
        view,
        viewProjection,
        inverseViewProjection
    );
}
