#include "world.hh"

#include "color.hh"
#include "intersection.hh"
#include "lighting.hh"
#include "mat3.hh"
#include "material.hh"
#include "ray.hh"
#include "shading.hh"
#include "sphere.hh"
#include "tform4.hh"

#include <algorithm> // sort
#include <cmath>     // nextafter
#include <utility>
#include <vector>

namespace wt {

world::world() noexcept = default;

world::world(std::vector<shape> shapes, pnt_light light) noexcept
    : shapes{std::move(shapes)}, light{std::move(light)} {}

world world::make_default() noexcept {
    std::vector<shape> shapes;
    shapes.emplace_back(
        sphere{tform4{}, material{.col = {.8f, 1.f, .6f}, .diffuse = .7f, .specular = .2f}});
    shapes.emplace_back(sphere{tform4{scale(.5f, .5f, .5f)}});
    return world{std::move(shapes),
                 pnt_light{.position = {-10.f, 10.f, -10.f}, .intensity = {1.f, 1.f, 1.f}}};
}

std::vector<intersection>::const_iterator
intersect(world const& world, ray const& world_ray,
          std::vector<intersection>& world_isecs) noexcept {
    for (auto shape = world.shapes.cbegin(); shape != world.shapes.cend(); ++shape) {
        ray const object_ray{inv_tform(*shape) * world_ray.origin,
                             // do not normalize the result, so we get the t straight for the world
                             // space without the need to convert it first from the object space
                             inv_tform(*shape) * world_ray.direction};
        intersect(*shape, object_ray, std::distance(world.shapes.cbegin(), shape), world_isecs);
    }

    // Find first smallest non-negative t, which represents closest intersection.
    std::sort(world_isecs.begin(), world_isecs.end());
    return std::upper_bound(
        world_isecs.cbegin(), world_isecs.cend(), 0.f,
        [](float predicate, intersection const& isec) { return predicate <= isec.t; });
}

color shade_hit(world const& world, shading const& shading_info,
                std::vector<intersection>& world_isecs) noexcept {
    return lighting(mater(world.shapes[shading_info.isec.shape_id]), world.light,
                    shading_info.isec_pnt, shading_info.eye, shading_info.normal,
                    is_shadowed(world, shading_info.over_pnt, world_isecs));
}

color color_at(world const& world, ray const& world_ray,
               std::vector<intersection>& world_isecs) noexcept {
    world_isecs.clear();
    auto hit = intersect(world, world_ray, world_isecs);
    if (hit == world_isecs.cend()) {
        return color{0, 0, 0};
    }
    shading sh{*hit, world_ray, world};
    return shade_hit(world, sh, world_isecs);
}

bool is_shadowed(world const& world, pnt3 const& world_point,
                 std::vector<intersection>& world_isecs) noexcept {
    vec3 const v = world.light.position - world_point;
    float const dist = magnitude(v);
    world_isecs.clear();
    auto hit = intersect(world, {world_point, normalize(v)}, world_isecs);
    return hit != world_isecs.cend() && hit->t < dist;
}

} // namespace wt
