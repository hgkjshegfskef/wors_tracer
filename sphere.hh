#pragma once

#include "tform4.hh"

#include <memory>

namespace wt {

// Sphere has radius 1 and is centered at the world origin (0,0,0).
struct sphere {
    std::shared_ptr<tform4> tform;

    sphere() noexcept;
    sphere(std::shared_ptr<tform4> tform) noexcept;
};

} // namespace wt
