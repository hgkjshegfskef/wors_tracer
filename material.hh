#pragma once

#include "color.hh"

namespace wt {

struct material {
    color color{1.f, 1.f, 1.f};
    float ambient{1.f};
    float diffuse{9.f};
    float specular{9.f};
    float shininess{200.f};
};

} // namespace wt
