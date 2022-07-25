#pragma once

#include "color.hh"
#include "pnt3.hh"

namespace wt {

struct pnt_light {
    pnt3 position;
    color intensity;
};

} // namespace wt
