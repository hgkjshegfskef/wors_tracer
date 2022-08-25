#include "solid_pattern.hh"

#include <utility> // move

namespace wt {

solid_pattern::solid_pattern(color first) noexcept
    : first{std::move(first)}, tform{}, inv_tform{} {}

solid_pattern::solid_pattern(color first, tform4 tform) noexcept
    : first{std::move(first)}, tform{std::move(tform)}, inv_tform{inverse(this->tform)} {}

} // namespace wt
