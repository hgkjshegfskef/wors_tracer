#include "solid_pattern.hh"

#include <utility> // move

namespace wt {

solid_pattern::solid_pattern(struct color color) noexcept
    : color{std::move(color)}, tform{}, inv_tform{} {}

solid_pattern::solid_pattern(struct color color, tform4 tform) noexcept
    : color{std::move(color)}, tform{std::move(tform)}, inv_tform{inverse(this->tform)} {}

solid_pattern::solid_pattern(float r, float g, float b) noexcept : solid_pattern{{r, g, b}} {}

solid_pattern::solid_pattern(float r, float g, float b, tform4 tform) noexcept
    : solid_pattern{{r, g, b}, std::move(tform)} {}

} // namespace wt
