#pragma once

#include "color.hh"
#include "pnt3.hh"
#include "tform4.hh"

#include <utility>

namespace wt {

struct test_pattern {
    tform4 tform;
    tform4 inv_tform;

    test_pattern() noexcept = default;
    test_pattern(tform4 tform) noexcept
        : tform{std::move(tform)}, inv_tform{inverse(this->tform)} {}
};

inline color pattern_at(test_pattern const& /*unused*/, pnt3 const& world_point) noexcept {
    return color{world_point.x, world_point.y, world_point.z};
}

inline tform4& inv_tform(test_pattern& pattern) noexcept { return pattern.inv_tform; }
inline tform4 const& inv_tform(test_pattern const& pattern) noexcept { return pattern.inv_tform; }

} // namespace wt
