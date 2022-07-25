#pragma once

#include "pnt3.hh"
#include "vec3.hh"

#include <array>
#include <optional>
#include <vector>

namespace wt {

struct intersection;
struct sphere;

struct ray {
    pnt3 origin;
    vec3 direction;

    ray() noexcept = default;
    ray(pnt3 const& p, vec3 const& v) noexcept;
};

// Intersect ray from world space with a sphere
std::optional<float> intersect(ray const& r, sphere const& s) noexcept;

// Intersect ray from object space (already transformed) with a sphere
std::optional<float> intersect_sphere(ray const& r) noexcept;

namespace v2 {
// Intersect and produce t compatible with world space.
std::array<intersection, 2> intersect(ray const& world_r, sphere const& s) noexcept;

// Find smallest positive intersection. Must check return value for empty().
template <typename Container> intersection hit(Container const& isecs) noexcept;

extern template intersection
hit<std::vector<intersection>>(std::vector<intersection> const&) noexcept;
extern template intersection
hit<std::array<intersection, 2>>(std::array<intersection, 2> const&) noexcept;
} // namespace v2

pnt3 position(pnt3 const& origin, vec3 const& direction, float t) noexcept;

// Distance from point q to line p+tv
float dist(pnt3 const& q, pnt3 const& p, vec3 const& v, bool v_is_normal = false) noexcept;

// intersection const* hit(std::vector<intersection> const& intersections) noexcept;

} // namespace wt
