#pragma once

namespace wt {

struct checker3d_pattern;
class tform4;
struct pnt3;
struct color;

tform4& inv_tform(checker3d_pattern& pattern) noexcept;
tform4 const& inv_tform(checker3d_pattern const& pattern) noexcept;

color pattern_at(checker3d_pattern const& pattern, pnt3 const& world_point) noexcept;

} // namespace wt
