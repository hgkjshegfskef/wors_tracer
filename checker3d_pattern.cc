#include "checker3d_pattern.hh"

#include <utility> // move

namespace wt {

checker3d_pattern::checker3d_pattern(color first, color second) noexcept
    : first{std::move(first)}, second{std::move(second)}, tform{}, inv_tform{} {}

checker3d_pattern::checker3d_pattern(color first, color second, tform4 tform) noexcept
    : first{std::move(first)}, second{std::move(second)}, tform{std::move(tform)},
      inv_tform{inverse(this->tform)} {}

} // namespace wt
