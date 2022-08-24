#pragma once

#include "color.hh"
#include "tform4.hh"

namespace wt {

struct gradient_pattern {
    color first;
    color second;
    tform4 tform;
    tform4 inv_tform;

    gradient_pattern(color first, color second) noexcept;
    gradient_pattern(color first, color second, tform4 tform) noexcept;
};

} // namespace wt
