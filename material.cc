#include "material.hh"

namespace wt {

material material::glass() noexcept { return {.transparency = 1, .refractive = 1.5}; }

} // namespace wt
