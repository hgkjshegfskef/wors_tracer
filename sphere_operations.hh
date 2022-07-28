#pragma once

#include <array> // array

namespace wt {

struct sphere;
struct vec3;
struct pnt3;
struct intersection;
struct ray;
class tform4;
struct material;

tform4& get_tform(sphere& sphere) noexcept;
tform4 const& get_tform(sphere const& sphere) noexcept;

material& get_mat(sphere& sphere) noexcept;
material const& get_mat(sphere const& sphere) noexcept;

vec3 normal_at(sphere const& sphere, pnt3 const& world_point) noexcept;

std::array<intersection, 2> intersect(sphere const& sphere, ray const& world_ray) noexcept;

} // namespace wt
