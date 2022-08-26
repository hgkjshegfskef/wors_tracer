#pragma once

#include "pattern.hh"
#include "tform4.hh"

namespace wt {

struct stripe_pattern {
    pattern first;
    pattern second;
    tform4 tform;
    tform4 inv_tform;

    stripe_pattern(pattern first, pattern second) noexcept;
    stripe_pattern(pattern first, pattern second, tform4 tform) noexcept;
};

} // namespace wt
