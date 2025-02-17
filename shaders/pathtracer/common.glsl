#version 450 core

layout (local_size_x = 16, local_size_y = 16) in;
layout (binding = 0, rgba32f) uniform image2D renderedImage;

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
