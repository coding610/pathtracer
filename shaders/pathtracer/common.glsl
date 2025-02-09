#version 450 core

struct Camera {
    vec3 position;
    vec3 direction;
    float fov;
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
