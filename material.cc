#include "material.hh"

namespace wt {

material material::glass() noexcept { return {.transparency = 1, .refraction_index = 1.5}; }

} // namespace wt
