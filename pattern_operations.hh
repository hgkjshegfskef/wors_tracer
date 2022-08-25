#pragma once

namespace wt {

class pattern;
class shape;
struct pnt3;
struct color;

color pattern_at(pattern const& pattern, shape const& shape, pnt3 const& world_point) noexcept;

} // namespace wt
