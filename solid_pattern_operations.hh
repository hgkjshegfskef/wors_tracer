#pragma once

namespace wt {

struct solid_pattern;
class tform4;
struct pnt3;
struct color;

tform4& inv_tform(solid_pattern& pattern) noexcept;
tform4 const& inv_tform(solid_pattern const& pattern) noexcept;

color pattern_at(solid_pattern const& pattern, pnt3 const& world_point) noexcept;

} // namespace wt
