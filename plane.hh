#pragma once

#include "material.hh"
#include "tform4.hh"

namespace wt {

// Plane is infinitely extended in xz, passing through the world origin (0,0,0).
struct plane {
    tform4 tform;
    tform4 inv_tform;
    struct material material;

    plane() noexcept;
    explicit plane(tform4 const& tform) noexcept;
    plane(tform4 const& tform, struct material const& material) noexcept;
};

} // namespace wt
