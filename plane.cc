#include "plane.hh"

namespace wt {

plane::plane() noexcept = default;

plane::plane(tform4 const& tform) noexcept : plane(tform, {}) {}

plane::plane(tform4 const& tform, struct material const& material) noexcept
    : tform{tform}, inv_tform{inverse(tform)}, material{material} {}

} // namespace wt
