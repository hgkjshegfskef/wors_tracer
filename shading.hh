#pragma once

#include "intersection.hh"
#include "pnt3.hh"
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
    shading(intersection const& isec, ray const& r, tform4 const& inv_tform) noexcept;
};

} // namespace wt
