#pragma once

#include "pattern.hh"
#include "tform4.hh"

namespace wt {

struct checker2d_pattern {
    pattern first;
    pattern second;
    tform4 tform;
    tform4 inv_tform;

    checker2d_pattern(pattern first, pattern second) noexcept;
    checker2d_pattern(pattern first, pattern second, tform4 tform) noexcept;
};

} // namespace wt
