#pragma once

#include "material.hh"
#include "tform4.hh"

#include <limits>

namespace wt {

// Cube is axis-aligned bounding box positioned at the world origin (0,0,0).
struct cylinder {
    tform4 tform;
    tform4 inv_tform;
    struct material material;
    bool cast_shadow; // to cast the shadow or not
    float min;
    float max;
    bool closed;

    cylinder(float min = -std::numeric_limits<float>::infinity(),
             float max = std::numeric_limits<float>::infinity(), bool closed = false) noexcept;
    explicit cylinder(tform4 tform, float min = -std::numeric_limits<float>::infinity(),
                      float max = std::numeric_limits<float>::infinity(),
                      bool closed = false) noexcept;
    cylinder(tform4 tform, struct material material,
             float min = -std::numeric_limits<float>::infinity(),
             float max = std::numeric_limits<float>::infinity(), bool closed = false) noexcept;
    cylinder(tform4 tform, struct material material, bool cast_shadow,
             float min = -std::numeric_limits<float>::infinity(),
             float max = std::numeric_limits<float>::infinity(), bool closed = false) noexcept;
};

} // namespace wt
