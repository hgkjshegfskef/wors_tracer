#pragma once

#include "tform4.hh"

namespace wt {

// Sphere has radius 1 and is centered at the world origin (0,0,0).
struct sphere {
    tform4 tform;

    sphere() noexcept;
    sphere(tform4 tform) noexcept;
};

} // namespace wt
