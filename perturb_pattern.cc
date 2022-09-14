#include "perturb_pattern.hh"

#include <utility> // move

namespace wt {

perturb_pattern::perturb_pattern(class pattern pattern, float scale, int octaves,
                                 float persistence) noexcept
    : pattern{std::move(pattern)}, octaves{octaves},
      persistence{persistence}, scale{scale}, tform{}, inv_tform{} {}

perturb_pattern::perturb_pattern(class pattern pattern, tform4 tform, float scale, int octaves,
                                 float persistence) noexcept
    : pattern{std::move(pattern)}, octaves{octaves}, persistence{persistence}, scale{scale},
      tform{std::move(tform)}, inv_tform{inverse(this->tform)} {}

} // namespace wt
