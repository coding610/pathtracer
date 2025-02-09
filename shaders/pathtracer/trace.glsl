#version 450 core

void setRayDirection(Camera camera, inout Ray ray) {
    // Ray direction from camera (local) perspective/direction
    vec3 rayCameraDirection = normalize(vec3(
        texCoord.x * aspectRatio * tan(radians(camera.fov / 2)),
        texCoord.y * tan(radians(camera.fov / 2)),
        -1
    ));

    // Apply camera direction/world direction
    mat3 rotationMatrix = createRotationMatrix(camera.position, camera.direction, vec3(0, 1, 0));
    vec3 rayWorldDirection = rotationMatrix * rayCameraDirection;
    ray.direction = normalize(rayWorldDirection);
}

void castRay(Camera camera, inout Ray ray) {
    // Todo :)
}
