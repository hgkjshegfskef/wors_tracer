#include "perturb_pattern.hh"

#include <utility> // move

namespace wt {

perturb_pattern::perturb_pattern(class pattern pattern, float scale) noexcept
    : pattern{std::move(pattern)}, scale{scale}, tform{}, inv_tform{} {}

perturb_pattern::perturb_pattern(class pattern pattern, tform4 tform, float scale) noexcept
    : pattern{std::move(pattern)}, scale{scale}, tform{std::move(tform)}, inv_tform{inverse(
                                                                              this->tform)} {}

} // namespace wt
