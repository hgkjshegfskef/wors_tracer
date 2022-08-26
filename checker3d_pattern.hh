#pragma once

#include "pattern.hh"
#include "tform4.hh"

namespace wt {

struct checker3d_pattern {
    pattern first;
    pattern second;
    tform4 tform;
    tform4 inv_tform;

    checker3d_pattern(pattern first, pattern second) noexcept;
    checker3d_pattern(pattern first, pattern second, tform4 tform) noexcept;
};

} // namespace wt
