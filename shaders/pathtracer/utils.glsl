#version 450 core

mat3 createRotationMatrix(vec3 from, vec3 to, vec3 worldUp) {
    vec3 forward = normalize(to - from);
    vec3 right = normalize(cross(worldUp, forward));
    vec3 up = cross(forward, right);
    return mat3(right, up, -forward);
}
