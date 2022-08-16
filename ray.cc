#include "ray.hh"

#include "intersection.hh"
#include "sphere.hh"

#include <cmath> // sqrt

namespace wt {

ray::ray(pnt3 p, vec3 v) noexcept : origin{std::move(p)}, direction{std::move(v)} {}

pnt3 position(pnt3 const& origin, vec3 const& direction, float t) noexcept {
    return origin + t * direction;
}

float dist(pnt3 const& q, pnt3 const& p, vec3 const& v, bool v_is_normal) noexcept {
    vec3 const a = cross(q - p, v);
    if (v_is_normal) {
        return std::sqrt(dot(a, a));
    }
    return std::sqrt(dot(a, a) / dot(v, v));
}

} // namespace wt
