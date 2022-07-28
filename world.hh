#pragma once

#include "pnt_light.hh"

#include <vector>

namespace wt {

struct ray;
struct intersection;
struct shading;
struct color;
class shape;

struct world {
    pnt_light light;
    std::vector<shape> shapes;

    world() noexcept = default;
    world(pnt_light const& light, std::vector<shape> const& shapes) noexcept;

    static world make_default() noexcept;
};

// Intersect ray from world space with the world
std::vector<intersection> intersect(ray const& world_r, world const& w) noexcept;

color shade_hit(world const& w, shading const& sh) noexcept;

color color_at(world const& w, ray const& r) noexcept;

bool is_shadowed(world const& w, pnt3 const& p) noexcept;

} // namespace wt
