#version 450 core

[[ common.glsl ]]
[[ utils.glsl ]]
[[ buffers.glsl ]]
[[ trace.glsl ]]

void main() {
    ivec2 globalID = ivec2(gl_GlobalInvocationID.xy);
    vec2 texCoord = vec2(globalID) / vec2(imageSize(renderedImage));

    Camera camera = { cameraPosition, cameraInverseViewProjection };
    Ray ray = {{0, 0, 0}, {0, 0, 0}}; setRayDirection(texCoord, camera, ray); castRay(camera, ray);

    imageStore(renderedImage, globalID, vec4(1, 0, 1, 1));
};
