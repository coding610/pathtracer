#version 450

layout(std140, binding = 0) uniform cameraBuffer {
    vec3 cameraPosition;
    vec3 cameraDirection;
    float cameraFov;
    float cameraFocalLength;
};

layout(std430, binding = 1) buffer sphereBuffer {
    Sphere spheres[];
};
