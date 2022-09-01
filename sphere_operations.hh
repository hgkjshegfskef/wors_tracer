#pragma once

#include <vector>

namespace wt {

struct sphere;
class tform4;
struct material;
struct ray;
struct intersection;
struct vec3;
struct pnt3;

tform4& tform(sphere& s) noexcept;
tform4 const& tform(sphere const& s) noexcept;

tform4& inv_tform(sphere& s) noexcept;
tform4 const& inv_tform(sphere const& s) noexcept;

struct material& mater(sphere& s) noexcept;
struct material const& mater(sphere const& s) noexcept;

bool& cast_shadow(sphere& s) noexcept;
bool const& cast_shadow(sphere const& s) noexcept;

void intersect(sphere const& /*unused*/, ray const& object_ray, unsigned shape_id,
               std::vector<intersection>& world_isecs) noexcept;

vec3 normal_at(sphere const& /*unused*/, pnt3 const& world_point, tform4 const& inv_tform) noexcept;

} // namespace wt
