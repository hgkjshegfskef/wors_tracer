#include "sphere.hh"

namespace wt {

sphere::sphere() noexcept = default;

sphere::sphere(tform4 const& tform) noexcept : tform{tform}, mat{} {}

sphere::sphere(tform4 const& tform, struct material const& material) noexcept
    : tform{tform}, mat{material} {}

} // namespace wt
