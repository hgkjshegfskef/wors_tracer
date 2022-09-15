#pragma once

#include <vector>

namespace wt {

struct cone;
class tform4;
struct ray;
struct intersection;
struct vec3;
struct pnt3;

// rest of the api is provided by the cylinder

void intersect(cone const& cone, ray const& object_ray, unsigned shape_id,
               std::vector<intersection>& world_isecs) noexcept;

vec3 normal_at(cone const& cone, pnt3 const& world_point, tform4 const& inv_tform) noexcept;

} // namespace wt
