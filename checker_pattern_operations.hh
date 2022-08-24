#pragma once

namespace wt {

struct checker_pattern;
class tform4;
struct pnt3;
class shape;
struct color;

color& first(checker_pattern& pattern) noexcept;
color const& first(checker_pattern const& pattern) noexcept;

color& second(checker_pattern& pattern) noexcept;
color const& second(checker_pattern const& pattern) noexcept;

tform4& tform(checker_pattern& pattern) noexcept;
tform4 const& tform(checker_pattern const& pattern) noexcept;

tform4& inv_tform(checker_pattern& pattern) noexcept;
tform4 const& inv_tform(checker_pattern const& pattern) noexcept;

color pattern_at(checker_pattern const& pattern, pnt3 const& world_point) noexcept;

color pattern_at(checker_pattern const& pattern, shape const& shape, pnt3 const& world_point) noexcept;

} // namespace wt
