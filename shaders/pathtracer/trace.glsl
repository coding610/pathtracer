#version 450 core

void directRay(Camera camera, inout Ray ray) {
    ray.direction = vec3(
        texCoord.x,
        texCoord.y,
        -camera.focalLength
    );
}

void castRay(Camera camera, inout Ray ray) {
    // Todo :)
}
