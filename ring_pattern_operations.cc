#include "ring_pattern_operations.hh"

#include "color.hh"
#include "pattern_operations.hh"
#include "pnt3.hh"
#include "ring_pattern.hh"
#include "shape.hh"
#include "stub_shape.hh"
#include "tform4.hh"

#include <cmath> // floor

namespace wt {

tform4& inv_tform(ring_pattern& pattern) noexcept { return pattern.inv_tform; }
tform4 const& inv_tform(ring_pattern const& pattern) noexcept { return pattern.inv_tform; }

color pattern_at(ring_pattern const& pattern, pnt3 const& world_point) noexcept {
    shape stub{stub_shape{}};
    if (int(std::floor(std::sqrt(world_point.x * world_point.x + world_point.z * world_point.z))) %
            2 ==
        0) {
        return pattern_at(pattern.first, stub, world_point);
    }
    return pattern_at(pattern.second, stub, world_point);
}

} // namespace wt
