#pragma once

#include "intersection.hh"
#include "pnt3.hh"
#include "vec3.hh"

namespace wt {

struct ray;
struct world;

struct shading {
    // everything is in world space
    intersection isec;
    pnt3 isec_pnt;
    pnt3 over_pnt;
    vec3 eye;
    vec3 normal;
    bool inside;

    shading() noexcept = default;
    shading(intersection const& isec, ray const& r, world const& w) noexcept;
};

} // namespace wt
