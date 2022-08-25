#include "gradient_pattern_operations.hh"

#include "color.hh"
#include "gradient_pattern.hh"
#include "pnt3.hh"
#include "tform4.hh"

#include <cmath> // floor

namespace wt {

tform4& inv_tform(gradient_pattern& pattern) noexcept { return pattern.inv_tform; }
tform4 const& inv_tform(gradient_pattern const& pattern) noexcept { return pattern.inv_tform; }

color pattern_at(gradient_pattern const& pattern, pnt3 const& world_point) noexcept {
    return pattern.first +
           (pattern.second - pattern.first) * (world_point.x - std::floor(world_point.x));
}

} // namespace wt
