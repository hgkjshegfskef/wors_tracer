#include "solid_pattern_operations.hh"

#include "color.hh"
#include "pnt3.hh"
#include "solid_pattern.hh"
#include "tform4.hh"

namespace wt {

tform4& inv_tform(solid_pattern& pattern) noexcept { return pattern.inv_tform; }
tform4 const& inv_tform(solid_pattern const& pattern) noexcept { return pattern.inv_tform; }

color pattern_at(solid_pattern const& pattern, pnt3 const& /*unused*/) noexcept {
    return pattern.color;
}

} // namespace wt
