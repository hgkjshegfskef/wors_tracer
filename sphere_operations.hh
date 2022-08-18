#pragma once

#include <vector>

namespace wt {

struct sphere;
class tform4;
struct material;
struct ray;
struct intersection;

tform4& tform(sphere& s) noexcept;
tform4 const& tform(sphere const& s) noexcept;

tform4& inv_tform(sphere& s) noexcept;
tform4 const& inv_tform(sphere const& s) noexcept;

struct material& mater(sphere& s) noexcept;
struct material const& mater(sphere const& s) noexcept;

void intersect(sphere const& /*unused*/, ray const& object_ray, unsigned shape_id,
               std::vector<intersection>& world_isecs) noexcept;

} // namespace wt
