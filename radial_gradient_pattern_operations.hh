#pragma once

namespace wt {

struct radial_gradient_pattern;
class tform4;
struct pnt3;
struct color;

tform4& inv_tform(radial_gradient_pattern& pattern) noexcept;
tform4 const& inv_tform(radial_gradient_pattern const& pattern) noexcept;

color pattern_at(radial_gradient_pattern const& pattern, pnt3 const& world_point) noexcept;

} // namespace wt
