/*
-- SceneEngine of App --

- Stores and manages objects.
- Provides data structures to populate SSBOs and UBOs.

TODO:
- Handle input
*/

#pragma once

#include <vector>

#include <models/object3D.hpp>


struct SceneCrate {
    std::vector<Sphere> objects;
};

class SceneEngine {
public:
    SceneEngine();
    ~SceneEngine();

    void buildCrate(SceneCrate& crate) const;
    void applyCrate(const SceneCrate& crate);

    void init(const SceneCrate& crate);
    void update();

private:
    std::vector<Sphere> objects; // On the heap to be able to add objects runtime
};
