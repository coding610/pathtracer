#include <engines/camera/cameraEngine.hpp>


//////////////////////
////// Builders //////
//////////////////////
CameraEngine::CameraEngine()  { }
CameraEngine::~CameraEngine() { }


////////////////////
////// Crates //////
////////////////////
void CameraEngine::buildCrate(CameraCrate& crate) const { crate = { position, direction, fov }; }
void CameraEngine::buildCrate(CameraBufferCrate& crate) const { crate = { position, direction, fov }; }
void CameraEngine::applyCrate(const CameraCrate& crate) { position = crate.position; direction = crate.direction; fov = crate.fov; }


//////////////////
////// Main //////
//////////////////
void CameraEngine::init(const CameraCrate& crate) {
    position = crate.position;
    direction = crate.direction;
    fov = crate.fov;
}

void CameraEngine::update() { }
