#pragma once

namespace wt {

struct perturb_pattern;
class tform4;
struct pnt3;
struct color;

tform4& inv_tform(perturb_pattern& pattern) noexcept;
tform4 const& inv_tform(perturb_pattern const& pattern) noexcept;

color pattern_at(perturb_pattern const& pattern, pnt3 const& world_point) noexcept;

} // namespace wt
