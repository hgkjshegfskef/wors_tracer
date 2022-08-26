#include "solid_pattern.hh"

#include <utility> // move

namespace wt {

solid_pattern::solid_pattern(struct color color) noexcept
    : color{std::move(color)}, tform{}, inv_tform{} {}

solid_pattern::solid_pattern(struct color color, tform4 tform) noexcept
    : color{std::move(color)}, tform{std::move(tform)}, inv_tform{inverse(this->tform)} {}

} // namespace wt
