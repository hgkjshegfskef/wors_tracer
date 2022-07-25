#pragma once

#include "intersection.hh"
#include "pnt3.hh"
#include "vec3.hh"

#include <optional>

namespace wt {

struct ray {
    pnt3 origin;
    vec3 direction;

    ray() noexcept = default;
    ray(pnt3 const& p, vec3 const& v) noexcept;
};

std::optional<float> intersect(ray const& r, sphere const& s) noexcept;

pnt3 position(pnt3 const& origin, vec3 const& direction, float t) noexcept;

// Distance from point q to line p+tv
float dist(pnt3 const& q, pnt3 const& p, vec3 const& v, bool v_is_normal = false) noexcept;

// intersection const* hit(std::vector<intersection> const& intersections) noexcept;

} // namespace wt
