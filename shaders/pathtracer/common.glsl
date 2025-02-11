#version 450 core

struct Camera {
    vec3 position;
    mat4 inverseViewProjection;
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
