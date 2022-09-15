#pragma once

#include "cylinder.hh"

namespace wt {

// Cone is double-napped, extending in Y.
struct cone : cylinder {
    using cylinder::cylinder;
};

} // namespace wt
