#include "radial_gradient_pattern_operations.hh"

#include "color.hh"
#include "pattern_operations.hh"
#include "pnt3.hh"
#include "radial_gradient_pattern.hh"
#include "shape.hh"
#include "stub_shape.hh"
#include "tform4.hh"

#include <cmath> // floor

namespace wt {

tform4& inv_tform(radial_gradient_pattern& pattern) noexcept { return pattern.inv_tform; }
tform4 const& inv_tform(radial_gradient_pattern const& pattern) noexcept {
    return pattern.inv_tform;
}

color pattern_at(radial_gradient_pattern const& pattern, pnt3 const& world_point) noexcept {
    float ring = std::sqrt(world_point.x * world_point.x + world_point.z * world_point.z)
        //                 - 1e-5f
        ; // this is a hack to account for fp impresicion
    shape stub{stub_shape{}};
    return pattern_at(pattern.first, stub, world_point) +
           (pattern_at(pattern.second, stub, world_point) -
            pattern_at(pattern.second, stub, world_point)) *
               (ring - std::floor(ring));
}

} // namespace wt
