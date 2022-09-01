#include "sphere.hh"

#include <utility> // move

namespace wt {

sphere::sphere() noexcept : sphere{{}} {}

sphere::sphere(tform4 tform) noexcept : sphere{std::move(tform), {}} {}

sphere::sphere(tform4 tform, struct material material) noexcept
    : sphere{std::move(tform), std::move(material), true} {}

sphere::sphere(tform4 tform, struct material material, bool cast_shadow) noexcept
    : tform{std::move(tform)}, inv_tform{inverse(this->tform)}, material{std::move(material)},
      cast_shadow{cast_shadow} {}

} // namespace wt
