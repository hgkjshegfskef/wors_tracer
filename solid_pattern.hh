#pragma once

#include "color.hh"
#include "tform4.hh"

namespace wt {

struct solid_pattern {
    color first;
    tform4 tform;
    tform4 inv_tform;

    solid_pattern(color first) noexcept;
    solid_pattern(color first, tform4 tform) noexcept;
};

} // namespace wt
