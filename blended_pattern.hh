#pragma once

#include "pattern.hh"
#include "tform4.hh"

namespace wt {

struct blended_pattern {
    pattern first;
    pattern second;
    tform4 tform;
    tform4 inv_tform;

    blended_pattern(pattern first, pattern second) noexcept;
    blended_pattern(pattern first, pattern second, tform4 tform) noexcept;
};

} // namespace wt
