#pragma once

#include "material.hh"
#include "tform4.hh"

namespace wt {

// Cube is axis-aligned bounding box positioned at the world origin (0,0,0).
struct cylinder {
    tform4 tform;
    tform4 inv_tform;
    struct material material;
    bool cast_shadow; // to cast the shadow or not

    cylinder() noexcept;
    explicit cylinder(tform4 tform) noexcept;
    cylinder(tform4 tform, struct material material) noexcept;
    cylinder(tform4 tform, struct material material, bool cast_shadow) noexcept;
};

} // namespace wt
