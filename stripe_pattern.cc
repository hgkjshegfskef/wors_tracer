#include "stripe_pattern.hh"

#include <utility>

namespace wt {

stripe_pattern::stripe_pattern(color first, color second) noexcept
    : first{std::move(first)}, second{std::move(second)}, tform{}, inv_tform{} {}

stripe_pattern::stripe_pattern(color first, color second, tform4 tform) noexcept
    : first{std::move(first)}, second{std::move(second)}, tform{std::move(tform)},
      inv_tform{inverse(this->tform)} {}

} // namespace wt
