#pragma once

namespace wt {

struct nested_checker2d_pattern;
class tform4;
struct pnt3;
struct color;

tform4& inv_tform(nested_checker2d_pattern& pattern) noexcept;
tform4 const& inv_tform(nested_checker2d_pattern const& pattern) noexcept;

color pattern_at(nested_checker2d_pattern const& pattern, pnt3 const& world_point) noexcept;

} // namespace wt
