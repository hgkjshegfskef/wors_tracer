#pragma once

namespace wt {

struct ring_pattern;
class tform4;
struct pnt3;
class shape;
struct color;

color& first(ring_pattern& pattern) noexcept;
color const& first(ring_pattern const& pattern) noexcept;

color& second(ring_pattern& pattern) noexcept;
color const& second(ring_pattern const& pattern) noexcept;

tform4& tform(ring_pattern& pattern) noexcept;
tform4 const& tform(ring_pattern const& pattern) noexcept;

tform4& inv_tform(ring_pattern& pattern) noexcept;
tform4 const& inv_tform(ring_pattern const& pattern) noexcept;

color pattern_at(ring_pattern const& pattern, pnt3 const& world_point) noexcept;

color pattern_at(ring_pattern const& pattern, shape const& shape, pnt3 const& world_point) noexcept;

} // namespace wt
