#include "nested_checker2d_pattern_operations.hh"

#include "color.hh"
#include "nested_checker2d_pattern.hh"
#include "pnt3.hh"
#include "tform4.hh"

#include <cmath> // floor

namespace wt {

tform4& inv_tform(nested_checker2d_pattern& pattern) noexcept { return pattern.inv_tform; }
tform4 const& inv_tform(nested_checker2d_pattern const& pattern) noexcept {
    return pattern.inv_tform;
}

color pattern_at(nested_checker2d_pattern const& pattern, pnt3 const& world_point) noexcept {
    if (int(std::floor(world_point.x) + std::floor(world_point.z)) % 2 == 0) {
        return pattern_at(pattern.first, pattern.inv_tform * world_point);
    }
    return pattern_at(pattern.second, pattern.inv_tform * world_point);
}

} // namespace wt
