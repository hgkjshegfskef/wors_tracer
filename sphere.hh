#pragma once

#include "material.hh"
#include "tform4.hh"

namespace wt {

struct vec3;
struct pnt3;

// Sphere has radius 1 and is centered at the world origin (0,0,0).
struct sphere {
    tform4 tform;
    material mat;

    sphere() noexcept;
    sphere(tform4 tform) noexcept;
    sphere(tform4 tform, struct material material) noexcept;
};

// Find normal in world space on sphere at the world point on the sphere.
vec3 normal_at(sphere const& s, pnt3 const& world_point) noexcept;

} // namespace wt
