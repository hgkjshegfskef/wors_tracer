#pragma once

#include <vector>

namespace wt {

struct plane;
class tform4;
struct material;
struct ray;
struct intersection;

tform4& tform(plane& p) noexcept;
tform4 const& tform(plane const& p) noexcept;

tform4& inv_tform(plane& p) noexcept;
tform4 const& inv_tform(plane const& p) noexcept;

struct material& mater(plane& p) noexcept;
struct material const& mater(plane const& p) noexcept;

void intersect(plane const& /*unused*/, ray const& object_ray, unsigned shape_id,
               std::vector<intersection>& world_isecs) noexcept;

} // namespace wt
