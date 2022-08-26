#pragma once

#include "material.hh"
#include "tform4.hh"

namespace wt {

// Used as a placeholder with default values when Shape interface is required
struct stub_shape {
    tform4 tform;
    tform4 inv_tform;
    struct material material;

    stub_shape() noexcept;
    explicit stub_shape(tform4 tform) noexcept;
    stub_shape(tform4 tform, struct material material) noexcept;
};

} // namespace wt
