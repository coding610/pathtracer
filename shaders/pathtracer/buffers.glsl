#version 450 core

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
