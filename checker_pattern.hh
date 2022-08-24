#pragma once

#include "color.hh"
#include "tform4.hh"

namespace wt {

struct checker_pattern {
    color first;
    color second;
    tform4 tform;
    tform4 inv_tform;

    checker_pattern(color first, color second) noexcept;
    checker_pattern(color first, color second, tform4 tform) noexcept;
};

} // namespace wt
