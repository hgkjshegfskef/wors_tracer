#include "look_at.hh"

#include <utility> // move

namespace wt {

look_at::look_at() noexcept : from{0, 1.5f, -5}, to{0, 1, 0}, up{0, 1, 0} {}

look_at::look_at(pnt3 from, pnt3 to, vec3 up) noexcept
    : from{std::move(from)}, to{std::move(to)}, up{std::move(up)} {}

} // namespace wt
