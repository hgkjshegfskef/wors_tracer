#include "blended_pattern_operations.hh"

#include "blended_pattern.hh"
#include "color.hh"
#include "pattern_operations.hh"
#include "pnt3.hh"
#include "shape.hh"
#include "stub_shape.hh"
#include "tform4.hh"

#include <cmath> // floor

namespace wt {

tform4& inv_tform(blended_pattern& pattern) noexcept { return pattern.inv_tform; }
tform4 const& inv_tform(blended_pattern const& pattern) noexcept { return pattern.inv_tform; }

color pattern_at(blended_pattern const& pattern, pnt3 const& world_point) noexcept {
    shape stub{stub_shape{}};
    return (pattern_at(pattern.first, stub, world_point) +
            pattern_at(pattern.second, stub, world_point)) /
           2.f;
}

} // namespace wt
