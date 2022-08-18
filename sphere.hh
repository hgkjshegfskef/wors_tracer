#pragma once

#include "material.hh"
#include "tform4.hh"

namespace wt {

// Sphere has radius 1 and is centered at the world origin (0,0,0).
struct sphere {
    tform4 tform;
    tform4 inv_tform;
    struct material material;

    sphere() noexcept;
    sphere(tform4 const& tform) noexcept;
    sphere(tform4 const& tform, struct material const& material) noexcept;
};

} // namespace wt
