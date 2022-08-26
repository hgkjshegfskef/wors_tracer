#pragma once

#include "pattern.hh"
#include "tform4.hh"

namespace wt {

struct gradient_pattern {
    pattern first;
    pattern second;
    tform4 tform;
    tform4 inv_tform;

    gradient_pattern(pattern first, pattern second) noexcept;
    gradient_pattern(pattern first, pattern second, tform4 tform) noexcept;
};

} // namespace wt
