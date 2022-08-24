#include "stripe_pattern_operations.hh"

#include "color.hh"
#include "pnt3.hh"
#include "shape.hh"
#include "stripe_pattern.hh"
#include "tform4.hh"

#include <cmath> // floor

namespace wt {

color& first(stripe_pattern& pattern) noexcept { return pattern.first; }
color const& first(stripe_pattern const& pattern) noexcept { return pattern.first; }

color& second(stripe_pattern& pattern) noexcept { return pattern.second; }
color const& second(stripe_pattern const& pattern) noexcept { return pattern.second; }

tform4& tform(stripe_pattern& pattern) noexcept { return pattern.tform; }
tform4 const& tform(stripe_pattern const& pattern) noexcept { return pattern.tform; }

tform4& inv_tform(stripe_pattern& pattern) noexcept { return pattern.inv_tform; }
tform4 const& inv_tform(stripe_pattern const& pattern) noexcept { return pattern.inv_tform; }

color stripe_at(stripe_pattern const& pattern, pnt3 const& world_point) noexcept {
    if (int(std::floor(world_point.x)) % 2 == 0) {
        return pattern.first;
    }
    return pattern.second;
}

color stripe_at(stripe_pattern const& pattern, shape const& shape,
                pnt3 const& world_point) noexcept {
    pnt3 const object_point = inv_tform(shape) * world_point;
    pnt3 const pattern_point = pattern.inv_tform * std::move(object_point);
    return stripe_at(pattern, std::move(pattern_point));
}

} // namespace wt
