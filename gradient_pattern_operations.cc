#include "gradient_pattern_operations.hh"

#include "color.hh"
#include "gradient_pattern.hh"
#include "pnt3.hh"
#include "shape.hh"
#include "tform4.hh"
#include "util.hh"

#include <cmath> // floor

namespace wt {

color& first(gradient_pattern& pattern) noexcept { return pattern.first; }
color const& first(gradient_pattern const& pattern) noexcept { return pattern.first; }

color& second(gradient_pattern& pattern) noexcept { return pattern.second; }
color const& second(gradient_pattern const& pattern) noexcept { return pattern.second; }

tform4& tform(gradient_pattern& pattern) noexcept { return pattern.tform; }
tform4 const& tform(gradient_pattern const& pattern) noexcept { return pattern.tform; }

tform4& inv_tform(gradient_pattern& pattern) noexcept { return pattern.inv_tform; }
tform4 const& inv_tform(gradient_pattern const& pattern) noexcept { return pattern.inv_tform; }

color pattern_at(gradient_pattern const& pattern, pnt3 const& world_point) noexcept {
    return blend(world_point, pattern.first, pattern.second);
}

color pattern_at(gradient_pattern const& pattern, shape const& shape,
                 pnt3 const& world_point) noexcept {
    pnt3 const object_point = inv_tform(shape) * world_point;
    pnt3 const pattern_point = pattern.inv_tform * std::move(object_point);
    return pattern_at(pattern, std::move(pattern_point));
}

} // namespace wt
