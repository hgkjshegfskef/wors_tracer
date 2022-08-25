#pragma once

namespace wt {

struct radial_gradient_pattern;
class tform4;
struct pnt3;
class shape;
struct color;

color& first(radial_gradient_pattern& pattern) noexcept;
color const& first(radial_gradient_pattern const& pattern) noexcept;

color& second(radial_gradient_pattern& pattern) noexcept;
color const& second(radial_gradient_pattern const& pattern) noexcept;

tform4& tform(radial_gradient_pattern& pattern) noexcept;
tform4 const& tform(radial_gradient_pattern const& pattern) noexcept;

tform4& inv_tform(radial_gradient_pattern& pattern) noexcept;
tform4 const& inv_tform(radial_gradient_pattern const& pattern) noexcept;

color pattern_at(radial_gradient_pattern const& pattern, pnt3 const& world_point) noexcept;

color pattern_at(radial_gradient_pattern const& pattern, shape const& shape,
                 pnt3 const& world_point) noexcept;

} // namespace wt
