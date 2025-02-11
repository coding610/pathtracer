/*
-- Material of object3D --

- Simple material struct
*/

#pragma once

#include <glm/glm.hpp>


struct Material {
    alignas(16) glm::vec3 albedo;
};
