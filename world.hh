#pragma once

#include "pnt_light.hh"
#include "shape.hh"

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
void intersect(world const& world, ray const& world_ray,
               std::vector<intersection>& world_isecs) noexcept;

color shade_hit(world const& world, shading const& shading_info,
                std::vector<intersection>& world_isecs) noexcept;

color color_at(world const& world, ray const& world_ray,
               std::vector<intersection>& world_isecs) noexcept;

bool is_shadowed(world const& world, pnt3 const& world_point,
                 std::vector<intersection>& world_isecs) noexcept;

} // namespace wt
