#include "stripe_pattern.hh"

#include "pnt3.hh"
#include "shape.hh"

#include <cmath>

namespace wt {

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
