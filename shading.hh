#pragma once

#include "intersection.hh"
#include "pnt3.hh"
#include "shape.hh"
#include "vec3.hh"

namespace wt {

struct ray;
class tform4;

struct shading {
    // everything is in world space
    intersection isec;
    pnt3 isec_pnt;
    pnt3 over_pnt;
    vec3 eye;
    vec3 normal;
    bool inside;

    shading() noexcept = default;
    shading(intersection const& isec, ray const& r, shape const& s) noexcept;
};

} // namespace wt
