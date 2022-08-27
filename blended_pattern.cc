#include "blended_pattern.hh"

#include <utility> // move

namespace wt {

blended_pattern::blended_pattern(pattern first, pattern second) noexcept
    : first{std::move(first)}, second{std::move(second)}, tform{}, inv_tform{} {}

blended_pattern::blended_pattern(pattern first, pattern second, tform4 tform) noexcept
    : first{std::move(first)}, second{std::move(second)}, tform{std::move(tform)},
      inv_tform{inverse(this->tform)} {}

} // namespace wt
