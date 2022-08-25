#include "nested_checker2d_pattern.hh"

namespace wt {

nested_checker2d_pattern::nested_checker2d_pattern(pattern first, pattern second) noexcept
    : first{std::move(first)}, second{std::move(second)}, tform{}, inv_tform{} {}

nested_checker2d_pattern::nested_checker2d_pattern(pattern first, pattern second,
                                                   tform4 tform) noexcept
    : first{std::move(first)}, second{std::move(second)}, tform{std::move(tform)},
      inv_tform{inverse(this->tform)} {}

} // namespace wt
