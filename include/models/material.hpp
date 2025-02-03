/*
-- Material of object3D --

- Simple material struct
*/

#pragma once

#include <vmmlib/vector.hpp>


struct Material {
    alignas(16) vmml::vec3f albedo;
};
