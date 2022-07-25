#pragma once

#include "material.hh"
#include "tform4.hh"

namespace wt {

// Sphere has radius 1 and is centered at the world origin (0,0,0).
struct sphere {
    tform4 tform;
    material mat;

    sphere() noexcept;
    sphere(tform4 tform) noexcept;
    sphere(tform4 tform, struct material material) noexcept;
};

} // namespace wt
