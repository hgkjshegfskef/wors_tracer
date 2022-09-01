#pragma once

#include "material.hh"
#include "tform4.hh"

namespace wt {

// Cube is axis-aligned bounding box positioned at the world origin (0,0,0).
struct cube {
    tform4 tform;
    tform4 inv_tform;
    struct material material;
    bool cast_shadow; // to cast the shadow or not

    cube() noexcept;
    explicit cube(tform4 tform) noexcept;
    cube(tform4 tform, struct material material) noexcept;
    cube(tform4 tform, struct material material, bool cast_shadow) noexcept;
};

} // namespace wt
