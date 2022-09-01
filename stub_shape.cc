#include "stub_shape.hh"

#include <utility> // move

namespace wt {

stub_shape::stub_shape() noexcept : stub_shape{{}} {}

stub_shape::stub_shape(tform4 tform) noexcept : stub_shape{std::move(tform), {}} {}

stub_shape::stub_shape(tform4 tform, struct material material) noexcept
    : stub_shape{std::move(tform), std::move(material), true} {}

stub_shape::stub_shape(tform4 tform, struct material material, bool cast_shadow) noexcept
    : tform{std::move(tform)}, inv_tform{inverse(this->tform)}, material{std::move(material)},
      cast_shadow{cast_shadow} {}

} // namespace wt
