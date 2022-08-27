#pragma once

#include "pattern.hh"
#include "tform4.hh"

namespace wt {

struct perturb_pattern {
    class pattern pattern;
    float scale;
    tform4 tform;
    tform4 inv_tform;

    perturb_pattern(class pattern pattern, float scale = .2f) noexcept;
    perturb_pattern(class pattern pattern, tform4 tform, float scale = .2f) noexcept;
};

} // namespace wt
