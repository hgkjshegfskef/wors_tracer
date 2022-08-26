#pragma once

#include "color.hh"
#include "tform4.hh"

namespace wt {

struct solid_pattern {
    struct color color;
    tform4 tform;
    tform4 inv_tform;

    solid_pattern(struct color color) noexcept;
    solid_pattern(struct color color, tform4 tform) noexcept;
};

} // namespace wt
