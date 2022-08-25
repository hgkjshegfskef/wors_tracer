#pragma once

namespace wt {

struct ring_pattern;
class tform4;
struct pnt3;
struct color;

tform4& inv_tform(ring_pattern& pattern) noexcept;
tform4 const& inv_tform(ring_pattern const& pattern) noexcept;

color pattern_at(ring_pattern const& pattern, pnt3 const& world_point) noexcept;

} // namespace wt
