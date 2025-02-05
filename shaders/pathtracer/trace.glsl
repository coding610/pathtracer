#version 450 core

void directRay(Camera& camera, Ray& ray) {
    ray.direction = vec3(
        texCoord.x,
        texCoord.y,
        -camera.focalLength
    );
}

void castRay(Camera& camera, Sphere[]& spheres, Ray& ray) {
}
