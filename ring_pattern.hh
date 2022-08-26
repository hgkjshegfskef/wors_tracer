#pragma once

#include "pattern.hh"
#include "tform4.hh"

namespace wt {

struct ring_pattern {
    pattern first;
    pattern second;
    tform4 tform;
    tform4 inv_tform;

    ring_pattern(pattern first, pattern second) noexcept;
    ring_pattern(pattern first, pattern second, tform4 tform) noexcept;
};

} // namespace wt
