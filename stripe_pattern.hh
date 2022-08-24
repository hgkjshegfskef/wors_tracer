#pragma once

#include "color.hh"
#include "tform4.hh"

namespace wt {

struct stripe_pattern {
    color first;
    color second;
    tform4 tform;
    tform4 inv_tform;

    stripe_pattern(color first, color second) noexcept;
    stripe_pattern(color first, color second, tform4 tform) noexcept;
};

} // namespace wt
