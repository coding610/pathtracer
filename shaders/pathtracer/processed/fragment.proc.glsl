#version 450 core
in vec2 texCoord;
out vec4 fragColor;


/* --- START INCLUDED FILE: shaders/pathtracer/common.glsl --- */

struct Camera {
    vec3 position;
    vec3 direction;
    float fov;
};

struct Material {
    vec3 albedo;
};

struct Sphere {
    vec3 position;
    float radius;
    Material material;
};

struct Ray {
    vec3 direction;
    vec3 albedo;
};

/* --- END INCLUDED FILE: shaders/pathtracer/common.glsl --- */


/* --- START INCLUDED FILE: shaders/pathtracer/utils.glsl --- */

mat3 createRotationMatrix(vec3 from, vec3 to, vec3 worldUp) {
    vec3 forward = normalize(to - from);
    vec3 right = normalize(cross(worldUp, forward));
    vec3 up = cross(forward, right);
    return mat3(right, up, -forward);
}

/* --- END INCLUDED FILE: shaders/pathtracer/utils.glsl --- */


/* --- START INCLUDED FILE: shaders/pathtracer/buffers.glsl --- */

uniform float windowWidth;
uniform float windowHeight;
uniform float aspectRatio;

layout(std140, binding = 0) uniform cameraBuffer {
    vec3 cameraPosition;
    vec3 cameraDirection;
    float cameraFov;
};

layout(std430, binding = 1) buffer sphereBuffer {
    Sphere spheres[];
};

/* --- END INCLUDED FILE: shaders/pathtracer/buffers.glsl --- */


/* --- START INCLUDED FILE: shaders/pathtracer/trace.glsl --- */

void setRayDirection(Camera camera, inout Ray ray) {
    // Ray direction from camera perspective
    vec3 rayCameraDirection = normalize(vec3(
        texCoord.x * aspectRatio * tan(radians(camera.fov / 2)),
        texCoord.y * tan(radians(camera.fov / 2)),
        -1
    ));

    mat3 rotationMatrix = createRotationMatrix(camera.position, camera.direction, vec3(0, 1, 0));
    vec3 rayWorldDirection = rotationMatrix * rayCameraDirection;
    ray.direction = normalize(rayWorldDirection);
}

void castRay(Camera camera, inout Ray ray) {
    // Todo :)
}

/* --- END INCLUDED FILE: shaders/pathtracer/trace.glsl --- */


void main() {
    Camera camera = { cameraPosition, cameraDirection, cameraFov };
    Ray ray; setRayDirection(camera, ray); castRay(camera, ray);
    fragColor = vec4(ray.direction, 1);
}
