#pragma once

#include "material.hh"
#include "tform4.hh"

namespace wt {

// Sphere has radius 1 and is centered at the world origin (0,0,0).
struct sphere {
    tform4 tform;
    tform4 inv_tform;
    struct material material;
    bool cast_shadow; // to cast the shadow or not

    sphere() noexcept;
    explicit sphere(tform4 tform) noexcept;
    sphere(tform4 tform, struct material material) noexcept;
    sphere(tform4 tform, struct material material, bool cast_shadow) noexcept;
};

} // namespace wt
