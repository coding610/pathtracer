#version 450 core

in vec2 texCoord;
out vec4 fragColor;

[[ common.glsl ]]
[[ utils.glsl ]]
[[ buffers.glsl ]]
[[ trace.glsl ]]

void main() {
    Camera camera = { cameraPosition, cameraInverseViewProjection };
    Ray ray; setRayDirection(camera, ray); castRay(camera, ray);
    fragColor = vec4(ray.direction, 1);
}
