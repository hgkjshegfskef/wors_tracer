#pragma once

#include <vector>

namespace wt {

struct cylinder;
class tform4;
struct material;
struct ray;
struct intersection;
struct vec3;
struct pnt3;

tform4& tform(cylinder& c) noexcept;
tform4 const& tform(cylinder const& c) noexcept;

tform4& inv_tform(cylinder& c) noexcept;
tform4 const& inv_tform(cylinder const& c) noexcept;

struct material& mater(cylinder& c) noexcept;
struct material const& mater(cylinder const& c) noexcept;

bool& cast_shadow(cylinder& c) noexcept;
bool const& cast_shadow(cylinder const& c) noexcept;

void intersect(cylinder const& /*unused*/, ray const& object_ray, unsigned shape_id,
               std::vector<intersection>& world_isecs) noexcept;

vec3 normal_at(cylinder const& /*unused*/, pnt3 const& world_point,
               tform4 const& inv_tform) noexcept;

} // namespace wt
