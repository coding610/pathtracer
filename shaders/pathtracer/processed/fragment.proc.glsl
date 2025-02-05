#version 450 core
in vec2 texCoord;
out vec4 fragColor;


/* --- START INCLUDED FILE: shaders/pathtracer/common.glsl --- */

struct Camera {
    vec3 position;
    vec3 direction;
    float fov;
    float focalLength;
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

/* --- END INCLUDED FILE: shaders/pathtracer/utils.glsl --- */


/* --- START INCLUDED FILE: shaders/pathtracer/buffers.glsl --- */

layout(std140, binding = 0) uniform cameraBuffer {
    vec3 cameraPosition;
    vec3 cameraDirection;
    float cameraFov;
    float cameraFocalLength;
};

layout(std430, binding = 1) buffer sphereBuffer {
    Sphere spheres[];
};

/* --- END INCLUDED FILE: shaders/pathtracer/buffers.glsl --- */


/* --- START INCLUDED FILE: shaders/pathtracer/trace.glsl --- */

void directRay(Camera& camera, Ray& ray) {
    ray.direction = vec3(
        texCoord.x,
        texCoord.y,
        -camera.focalLength
    );
}

void castRay(Camera& camera, Sphere[]& spheres, Ray& ray) {
}

/* --- END INCLUDED FILE: shaders/pathtracer/trace.glsl --- */


void main() {
    Camera camera = {cameraPosition, cameraDirection, cameraFov, cameraFocalLength};
    Ray ray; directRay(camera, ray); castRay(camera, spheres, ray);
    fragColor = vec4(ray.albedo, 1);
}
