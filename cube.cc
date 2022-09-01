#include "cube.hh"

#include <utility> // move

namespace wt {

cube::cube() noexcept : cube{{}} {}

cube::cube(tform4 tform) noexcept : cube{std::move(tform), {}} {}

cube::cube(tform4 tform, struct material material) noexcept
    : cube{std::move(tform), std::move(material), true} {}

cube::cube(tform4 tform, struct material material, bool cast_shadow) noexcept
    : tform{std::move(tform)}, inv_tform{inverse(this->tform)}, material{std::move(material)},
      cast_shadow{cast_shadow} {}

} // namespace wt
