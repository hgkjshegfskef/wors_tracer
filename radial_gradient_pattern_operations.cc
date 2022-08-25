#include "radial_gradient_pattern_operations.hh"

#include "color.hh"
#include "pnt3.hh"
#include "radial_gradient_pattern.hh"
#include "shape.hh"
#include "tform4.hh"
#include "util.hh"

#include <cmath> // floor

namespace wt {

color& first(radial_gradient_pattern& pattern) noexcept { return pattern.first; }
color const& first(radial_gradient_pattern const& pattern) noexcept { return pattern.first; }

color& second(radial_gradient_pattern& pattern) noexcept { return pattern.second; }
color const& second(radial_gradient_pattern const& pattern) noexcept { return pattern.second; }

tform4& tform(radial_gradient_pattern& pattern) noexcept { return pattern.tform; }
tform4 const& tform(radial_gradient_pattern const& pattern) noexcept { return pattern.tform; }

tform4& inv_tform(radial_gradient_pattern& pattern) noexcept { return pattern.inv_tform; }
tform4 const& inv_tform(radial_gradient_pattern const& pattern) noexcept {
    return pattern.inv_tform;
}

color pattern_at(radial_gradient_pattern const& pattern, pnt3 const& world_point) noexcept {
    float ring = std::sqrt(world_point.x * world_point.x + world_point.z * world_point.z) -
                 1e-5f // this is a hack to account for fp impresicion
        ;
    return pattern.first + (pattern.second - pattern.first) * (ring - std::floor(ring));
}

color pattern_at(radial_gradient_pattern const& pattern, shape const& shape,
                 pnt3 const& world_point) noexcept {
    return pattern_at(pattern, inv_tform(pattern) * inv_tform(shape) * world_point);
}

} // namespace wt
