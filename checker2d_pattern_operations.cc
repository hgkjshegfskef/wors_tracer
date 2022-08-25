#include "checker2d_pattern_operations.hh"

#include "checker2d_pattern.hh"
#include "color.hh"
#include "pnt3.hh"
#include "shape.hh"
#include "tform4.hh"

#include <cmath> // floor

namespace wt {

color& first(checker2d_pattern& pattern) noexcept { return pattern.first; }
color const& first(checker2d_pattern const& pattern) noexcept { return pattern.first; }

color& second(checker2d_pattern& pattern) noexcept { return pattern.second; }
color const& second(checker2d_pattern const& pattern) noexcept { return pattern.second; }

tform4& tform(checker2d_pattern& pattern) noexcept { return pattern.tform; }
tform4 const& tform(checker2d_pattern const& pattern) noexcept { return pattern.tform; }

tform4& inv_tform(checker2d_pattern& pattern) noexcept { return pattern.inv_tform; }
tform4 const& inv_tform(checker2d_pattern const& pattern) noexcept { return pattern.inv_tform; }

color pattern_at(checker2d_pattern const& pattern, pnt3 const& world_point) noexcept {
    if (int(std::floor(world_point.x) + std::floor(world_point.z)) % 2 == 0) {
        return pattern.first;
    }
    return pattern.second;
}

color pattern_at(checker2d_pattern const& pattern, shape const& shape,
                 pnt3 const& world_point) noexcept {
    pnt3 const object_point = inv_tform(shape) * world_point;
    pnt3 const pattern_point = pattern.inv_tform * std::move(object_point);
    return pattern_at(pattern, std::move(pattern_point));
}

} // namespace wt
