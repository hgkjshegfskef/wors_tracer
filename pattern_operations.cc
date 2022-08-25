#include "pattern_operations.hh"

#include "color.hh"
#include "pattern.hh"
#include "pnt3.hh"
#include "shape.hh"
#include "tform4.hh"

namespace wt {

color pattern_at(pattern const& pattern, shape const& shape, pnt3 const& world_point) noexcept {
    return pattern_at(pattern, inv_tform(pattern) * inv_tform(shape) * world_point);
}

} // namespace wt
