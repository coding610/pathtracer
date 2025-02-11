/*
-- Object3D of partial SceneEngine --

- Includes a base class, Object3D, only used for allowing types. Has no real value.
- Includes all other object types.
*/

#pragma once

#include <glm/glm.hpp>
#include <models/material.hpp>


struct Sphere {
    glm::vec3 position;
    float radius;
    Material material;
};
