#pragma once

#include "color.hh"
#include "tform4.hh"

namespace wt {

struct stripe_pattern {
    color first;
    color second;
    tform4 tform;
    tform4 inv_tform;
};

struct pnt3;
color stripe_at(stripe_pattern const& pattern, pnt3 const& world_point) noexcept;

class shape;
color stripe_at(stripe_pattern const& pattern, shape const& shape,
                pnt3 const& world_point) noexcept;

} // namespace wt
