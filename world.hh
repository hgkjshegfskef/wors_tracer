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
    std::vector<shape> shapes;
    pnt_light light;

    world() noexcept;
    world(std::vector<shape> shapes, pnt_light light) noexcept;

    static world make_default() noexcept;
};

// unsigned shape_id_from_sphere(unsigned sphere_idx) noexcept;

// Intersect ray from world space with the world

std::vector<intersection>::const_iterator
intersect(world const& world, ray const& world_ray,
          std::vector<intersection>& world_isecs) noexcept;

color shade_hit(world const& world, shading const& shading_info,
                std::vector<intersection>& world_isecs, unsigned remaining) noexcept;

color color_at(world const& world, ray const& world_ray, std::vector<intersection>& world_isecs,
               unsigned remaining) noexcept;

bool is_shadowed(world const& world, pnt3 const& world_point,
                 std::vector<intersection>& world_isecs) noexcept;

color reflected_color(world const& world, shading const& shading_info,
                      std::vector<intersection>& world_isecs, unsigned remaining = 5) noexcept;

color refracted_color(world const& world, shading const& shading_info,
                      std::vector<intersection>& world_isecs, unsigned remaining) noexcept;

float schlick(shading const& shading_info) noexcept;

} // namespace wt
