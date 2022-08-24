#pragma once

#include "color.hh"
#include "tform4.hh"

namespace wt {

struct ring_pattern {
    color first;
    color second;
    tform4 tform;
    tform4 inv_tform;

    ring_pattern(color first, color second) noexcept;
    ring_pattern(color first, color second, tform4 tform) noexcept;
};

} // namespace wt
