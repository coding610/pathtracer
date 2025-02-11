#version 450 core
in vec2 texCoord;
out vec4 fragColor;


/* --- START INCLUDED FILE: shaders/pathtracer/common.glsl --- */

struct Camera {
    vec3 position;
    mat4 inverseViewProjection;
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
    mat4 cameraInverseViewProjection;
};

layout(std430, binding = 1) buffer sphereBuffer {
    Sphere spheres[];
};

/* --- END INCLUDED FILE: shaders/pathtracer/buffers.glsl --- */


/* --- START INCLUDED FILE: shaders/pathtracer/trace.glsl --- */

void setRayDirection(Camera camera, inout Ray ray) {
    vec4 clipCoords = vec4(texCoord, -1.0, 1.0);
    vec4 worldCoords = camera.inverseViewProjection * clipCoords;
    worldCoords /= worldCoords.w;
    ray.direction = normalize(worldCoords.xyz - camera.position);
}

void castRay(Camera camera, inout Ray ray) {
    // Todo :)
}

/* --- END INCLUDED FILE: shaders/pathtracer/trace.glsl --- */


void main() {
    Camera camera = { cameraPosition, cameraInverseViewProjection };
    Ray ray; setRayDirection(camera, ray); castRay(camera, ray);
    fragColor = vec4(ray.direction, 1);
}
