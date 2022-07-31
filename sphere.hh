#pragma once

#include "material.hh"
#include "tform4.hh"

#include <array>

namespace wt {

class tform4;
struct material;
struct vec3;
struct pnt3;
struct ray;
struct intersection;

// Sphere has radius 1 and is centered at the world origin (0,0,0).
struct sphere {
    tform4 tform;
    material mat;

    sphere() noexcept;
    sphere(tform4 const& tform) noexcept;
    sphere(tform4 const& tform, struct material const& material) noexcept;
};

tform4& get_tform(sphere& sphere) noexcept;
tform4 const& get_tform(sphere const& sphere) noexcept;

material& get_mat(sphere& sphere) noexcept;
material const& get_mat(sphere const& sphere) noexcept;

vec3 normal_at(sphere const& sphere, pnt3 const& world_point) noexcept;

std::array<intersection, 2> intersect(sphere const& sphere, ray const& world_ray) noexcept;

} // namespace wt
