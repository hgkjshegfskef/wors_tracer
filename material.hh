#pragma once

#include "color.hh"

namespace wt {

struct material {
    color col{1.f, 1.f, 1.f};
    float ambient{.1f};
    float diffuse{.9f};
    float specular{.9f};
    float shininess{200.f};
};

} // namespace wt
