#pragma once

#include "pnt_light.hh"
#include "sphere.hh"

#include <vector>

namespace wt {

struct ray;
struct intersection;
struct shading;
struct color;

struct world {
    pnt_light light;
    std::vector<sphere> shapes;

    world() noexcept = default;
    world(pnt_light const& light, std::vector<sphere> const& spheres) noexcept;

    static world make_default() noexcept;
};

// Intersect ray from world space with the world
std::vector<intersection> intersect(ray const& world_r, world const& w) noexcept;

color shade_hit(world const& w, shading const& sh) noexcept;

color color_at(world const& w, ray const& r) noexcept;

} // namespace wt
