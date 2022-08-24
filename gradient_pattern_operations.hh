#pragma once

namespace wt {

struct gradient_pattern;
class tform4;
struct pnt3;
class shape;
struct color;

color& first(gradient_pattern& pattern) noexcept;
color const& first(gradient_pattern const& pattern) noexcept;

color& second(gradient_pattern& pattern) noexcept;
color const& second(gradient_pattern const& pattern) noexcept;

tform4& tform(gradient_pattern& pattern) noexcept;
tform4 const& tform(gradient_pattern const& pattern) noexcept;

tform4& inv_tform(gradient_pattern& pattern) noexcept;
tform4 const& inv_tform(gradient_pattern const& pattern) noexcept;

color pattern_at(gradient_pattern const& pattern, pnt3 const& world_point) noexcept;

color pattern_at(gradient_pattern const& pattern, shape const& shape,
                 pnt3 const& world_point) noexcept;

} // namespace wt
