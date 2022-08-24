#pragma once

namespace wt {

struct stripe_pattern;
class tform4;
struct pnt3;
class shape;
struct color;

color& first(stripe_pattern& pattern) noexcept;
color const& first(stripe_pattern const& pattern) noexcept;

color& second(stripe_pattern& pattern) noexcept;
color const& second(stripe_pattern const& pattern) noexcept;

tform4& tform(stripe_pattern& pattern) noexcept;
tform4 const& tform(stripe_pattern const& pattern) noexcept;

tform4& inv_tform(stripe_pattern& pattern) noexcept;
tform4 const& inv_tform(stripe_pattern const& pattern) noexcept;

color stripe_at(stripe_pattern const& pattern, pnt3 const& world_point) noexcept;

color stripe_at(stripe_pattern const& pattern, shape const& shape,
                pnt3 const& world_point) noexcept;

} // namespace wt
