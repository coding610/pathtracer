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


namespace SceneUtils {

struct SceneCrate {
    std::vector<Sphere> objects;
};

}

class SceneEngine {
public:
    SceneEngine();
    ~SceneEngine();

    void buildCrate(SceneUtils::SceneCrate& crate) const;
    void applyCrate(const SceneUtils::SceneCrate& crate);

    void init(const SceneUtils::SceneCrate& crate);
    void update();

private:
    std::vector<Sphere> objects; // On the heap to be able to add objects runtime
};
