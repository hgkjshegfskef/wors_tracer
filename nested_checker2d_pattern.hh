#pragma once

#include "pattern.hh"
#include "tform4.hh"

namespace wt {

struct nested_checker2d_pattern {
    pattern first;
    pattern second;
    tform4 tform;
    tform4 inv_tform;

    nested_checker2d_pattern(pattern first, pattern second) noexcept;
    nested_checker2d_pattern(pattern first, pattern second, tform4 tform) noexcept;
};

} // namespace wt
