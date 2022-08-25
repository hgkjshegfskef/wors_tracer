#include "checker2d_pattern_operations.hh"

#include "checker2d_pattern.hh"
#include "color.hh"
#include "pnt3.hh"
#include "tform4.hh"

#include <cmath> // floor

namespace wt {

tform4& inv_tform(checker2d_pattern& pattern) noexcept { return pattern.inv_tform; }
tform4 const& inv_tform(checker2d_pattern const& pattern) noexcept { return pattern.inv_tform; }

color pattern_at(checker2d_pattern const& pattern, pnt3 const& world_point) noexcept {
    if (int(std::floor(world_point.x) + std::floor(world_point.z)) % 2 == 0) {
        return pattern.first;
    }
    return pattern.second;
}

} // namespace wt
