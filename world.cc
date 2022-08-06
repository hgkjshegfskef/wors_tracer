#include "world.hh"

#include "color.hh"
#include "intersection.hh"
#include "lighting.hh"
#include "mat3.hh"
#include "material.hh"
#include "ray.hh"
#include "shading.hh"
#include "shape.hh"
#include "sphere.hh"
#include "tform4.hh"

#include <algorithm> // sort
#include <vector>

namespace wt {

world::world(pnt_light const& light, std::vector<shape> const& shapes) noexcept
    : light{light}, shapes{shapes} {}

world world::make_default() noexcept {
    std::vector<shape> shapes;
    shapes.emplace_back(
        sphere{tform4{}, material{.col = {.8f, 1.f, .6f}, .diffuse = .7f, .specular = .2f}});
    shapes.emplace_back(sphere{tform4{scale(.5f, .5f, .5f)}});
    return world{pnt_light{.position = {-10.f, 10.f, -10.f}, .intensity = {1.f, 1.f, 1.f}},
                 std::move(shapes)};
}

void intersect(world const& world, ray const& world_ray,
               std::vector<intersection>& world_isecs) noexcept {
    for (auto const& s : world.shapes) {
        auto const isecs = intersect(s, world_ray);
        world_isecs.insert(world_isecs.cend(), isecs.cbegin(), isecs.cend());
    }
}

color shade_hit(world const& world, shading const& shading_info,
                std::vector<intersection>& world_isecs) noexcept {
    return lighting(get_material(*shading_info.isec.s), world.light, shading_info.isec_pnt,
                    shading_info.eye, shading_info.normal,
                    is_shadowed(world, shading_info.over_pnt, world_isecs));
}

color color_at(world const& world, ray const& world_ray,
               std::vector<intersection>& world_isecs) noexcept {
    world_isecs.clear();
    intersect(world, world_ray, world_isecs);
    intersection const isec = hit(world_isecs);
    if (isec.empty()) {
        return color{0, 0, 0};
    }
    shading sh{isec, world_ray};
    return shade_hit(world, sh, world_isecs);
}

bool is_shadowed(world const& world, pnt3 const& world_point,
                 std::vector<intersection>& world_isecs) noexcept {
    vec3 const v = world.light.position - world_point;
    float const dist = magnitude(v);
    vec3 const direction = normalize(v);
    ray const r{world_point, direction};
    world_isecs.clear();
    intersect(world, r, world_isecs);
    intersection isec = hit(world_isecs);
    if (!isec.empty() && isec.t < dist) {
        return true;
    }
    return false;
}

} // namespace wt
