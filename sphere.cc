#include "sphere.hh"

namespace wt {

sphere::sphere() noexcept = default;

sphere::sphere(tform4 const& tform) noexcept : sphere(tform, {}) {}

sphere::sphere(tform4 const& tform, struct material const& material) noexcept
    : tform{tform}, inv_tform{inverse(tform)}, material{material} {}

} // namespace wt
