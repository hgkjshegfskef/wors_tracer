#include "cylinder.hh"

#include <utility> // move

namespace wt {

cylinder::cylinder() noexcept : cylinder{{}} {}

cylinder::cylinder(tform4 tform) noexcept : cylinder{std::move(tform), {}} {}

cylinder::cylinder(tform4 tform, struct material material) noexcept
    : cylinder{std::move(tform), std::move(material), true} {}

cylinder::cylinder(tform4 tform, struct material material, bool cast_shadow) noexcept
    : tform{std::move(tform)}, inv_tform{inverse(this->tform)}, material{std::move(material)},
      cast_shadow{cast_shadow} {}

} // namespace wt
