#include "plane.hh"

#include <utility> // move

namespace wt {

plane::plane() noexcept : plane{{}} {}

plane::plane(tform4 tform) noexcept : plane{std::move(tform), {}} {}

plane::plane(tform4 tform, struct material material) noexcept
    : plane{std::move(tform), std::move(material), true} {}

plane::plane(tform4 tform, struct material material, bool cast_shadow) noexcept
    : tform{std::move(tform)}, inv_tform{inverse(this->tform)}, material{std::move(material)},
      cast_shadow{cast_shadow} {}

} // namespace wt
