#pragma once

#include <vector>

namespace wt {

struct plane;
class tform4;
struct material;
struct ray;
struct intersection;
struct vec3;
struct pnt3;

tform4& tform(plane& p) noexcept;
tform4 const& tform(plane const& p) noexcept;

tform4& inv_tform(plane& p) noexcept;
tform4 const& inv_tform(plane const& p) noexcept;

struct material& mater(plane& p) noexcept;
struct material const& mater(plane const& p) noexcept;

bool& cast_shadow(plane& p) noexcept;
bool const& cast_shadow(plane const& p) noexcept;

void intersect(plane const& /*unused*/, ray const& object_ray, unsigned shape_id,
               std::vector<intersection>& world_isecs) noexcept;

vec3 normal_at(plane const& /*unused*/, pnt3 const& /*unused*/, tform4 const& inv_tform) noexcept;

} // namespace wt
