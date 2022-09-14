#include "cylinder.hh"

#include <utility> // move

namespace wt {

cylinder::cylinder(float min, float max, bool closed) noexcept : cylinder{{}, min, max, closed} {}

cylinder::cylinder(tform4 tform, float min, float max, bool closed) noexcept
    : cylinder{std::move(tform), {}, min, max, closed} {}

cylinder::cylinder(tform4 tform, struct material material, float min, float max,
                   bool closed) noexcept
    : cylinder{std::move(tform), std::move(material), true, min, max, closed} {}

cylinder::cylinder(tform4 tform, struct material material, bool cast_shadow, float min, float max,
                   bool closed) noexcept
    : tform{std::move(tform)}, inv_tform{inverse(this->tform)}, material{std::move(material)},
      cast_shadow{cast_shadow}, min{min}, max{max}, closed{closed} {}

} // namespace wt
