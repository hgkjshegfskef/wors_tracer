#pragma once

#include <vector>

namespace wt {

struct cube;
class tform4;
struct material;
struct ray;
struct intersection;
struct vec3;
struct pnt3;

tform4& tform(cube& c) noexcept;
tform4 const& tform(cube const& c) noexcept;

tform4& inv_tform(cube& c) noexcept;
tform4 const& inv_tform(cube const& c) noexcept;

struct material& mater(cube& c) noexcept;
struct material const& mater(cube const& c) noexcept;

bool& cast_shadow(cube& c) noexcept;
bool const& cast_shadow(cube const& c) noexcept;

void intersect(cube const& /*unused*/, ray const& object_ray, unsigned shape_id,
               std::vector<intersection>& world_isecs) noexcept;

vec3 normal_at(cube const& /*unused*/, pnt3 const& world_point, tform4 const& inv_tform) noexcept;

} // namespace wt
