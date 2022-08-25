#pragma once

#include "color.hh"
#include "tform4.hh"

namespace wt {

struct radial_gradient_pattern {
    color first;
    color second;
    tform4 tform;
    tform4 inv_tform;

    radial_gradient_pattern(color first, color second) noexcept;
    radial_gradient_pattern(color first, color second, tform4 tform) noexcept;
};

} // namespace wt
