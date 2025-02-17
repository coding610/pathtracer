#version 450 core

void setRayDirection(vec2 texCoord, Camera camera, inout Ray ray) {
    vec4 clipCoords = vec4(texCoord, -1.0, 1.0);
    vec4 worldCoords = camera.inverseViewProjection * clipCoords;
    worldCoords /= worldCoords.w;
    ray.direction = normalize(worldCoords.xyz - camera.position);
}

void castRay(Camera camera, inout Ray ray) {
    // Todo :)
}
