#pragma once

#include "intersection.hh"
#include "pnt3.hh"
#include "shape.hh"
#include "vec3.hh"

#include <vector>

namespace wt {

struct ray;
class tform4;

struct shading {
    // everything is in world space
    intersection isec;
    pnt3 isec_pnt;
    pnt3 over_pnt;
    pnt3 under_pnt;
    vec3 eye;
    vec3 normal;
    vec3 reflect;
    float n1;
    float n2;
    bool inside;

    shading() noexcept = default;
    shading(std::vector<shape> const& shapes, ray const& r,
            std::vector<intersection> const& world_isecs, intersection hit) noexcept;
};

} // namespace wt
