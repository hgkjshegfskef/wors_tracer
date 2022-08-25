#pragma once

namespace wt {

struct checker3d_pattern;
class tform4;
struct pnt3;
class shape;
struct color;

color& first(checker3d_pattern& pattern) noexcept;
color const& first(checker3d_pattern const& pattern) noexcept;

color& second(checker3d_pattern& pattern) noexcept;
color const& second(checker3d_pattern const& pattern) noexcept;

tform4& tform(checker3d_pattern& pattern) noexcept;
tform4 const& tform(checker3d_pattern const& pattern) noexcept;

tform4& inv_tform(checker3d_pattern& pattern) noexcept;
tform4 const& inv_tform(checker3d_pattern const& pattern) noexcept;

color pattern_at(checker3d_pattern const& pattern, pnt3 const& world_point) noexcept;

color pattern_at(checker3d_pattern const& pattern, shape const& shape, pnt3 const& world_point) noexcept;

} // namespace wt
