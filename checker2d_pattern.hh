#pragma once

#include "color.hh"
#include "tform4.hh"

namespace wt {

struct checker2d_pattern {
    color first;
    color second;
    tform4 tform;
    tform4 inv_tform;

    checker2d_pattern(color first, color second) noexcept;
    checker2d_pattern(color first, color second, tform4 tform) noexcept;
};

} // namespace wt
