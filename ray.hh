#pragma once

#include "pnt3.hh"
#include "vec3.hh"

#include <array>
#include <optional>
#include <vector>

namespace wt {

struct intersection;

struct ray {
    pnt3 origin;
    vec3 direction;

    ray() noexcept = default;
    ray(pnt3 p, vec3 v) noexcept;
};

pnt3 position(pnt3 const& origin, vec3 const& direction, float t) noexcept;

// Distance from point q to line p+tv
float dist(pnt3 const& q, pnt3 const& p, vec3 const& v, bool v_is_normal = false) noexcept;

} // namespace wt
