#pragma once

#include "pnt3.hh"
#include "vec3.hh"

namespace wt {

struct look_at {
    pnt3 from;
    pnt3 to;
    vec3 up;

    look_at() noexcept;
    look_at(pnt3 from, pnt3 to, vec3 up) noexcept;
};

} // namespace wt
