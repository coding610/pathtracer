#version 430 core

in vec2 texCoord;
out vec4 fragColor;

/////////////////////
////// Structs //////
/////////////////////
struct Camera {
    vec3 position;
    vec3 direction;
    float fov;
    float focalPoint;
};

struct Material {
    vec3 albedo;
};

struct Sphere {
    vec3 position;
    float radius;
    Material material;
};

/////////////////////
////// Buffers //////
/////////////////////
layout(std140, binding = 0) uniform cameraBuffer {
    vec3 cameraPosition;
    vec3 cameraDirection;
    float cameraFov;
    float cameraFocalPoint;
};

layout(std430, binding = 1) buffer sphereBuffer {
    Sphere spheres[];
};

//////////////////
////// Main //////
//////////////////
void main() {
    Camera camera = {cameraPosition, cameraDirection, cameraFov, cameraFocalPoint};
    fragColor = vec4(spheres[0].radius, 0, 0, 1);
}
