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
#include <cmath>     // nextafter
#include <utility>
#include <vector>

namespace wt {

world::world() noexcept = default;

world::world(struct spheres spheres, pnt_light light) noexcept
    : spheres{std::move(spheres)}, light{std::move(light)} {}

// world world::make_default() noexcept {
//     std::vector<shape> shapes;
//     shapes.emplace_back(
//         sphere{tform4{}, material{.col = {.8f, 1.f, .6f}, .diffuse = .7f, .specular = .2f}});
//     shapes.emplace_back(sphere{tform4{scale(.5f, .5f, .5f)}});
//     return world{pnt_light{.position = {-10.f, 10.f, -10.f}, .intensity = {1.f, 1.f, 1.f}},
//                  std::move(shapes)};
// }

std::vector<intersection>::const_iterator
intersect(world const& world, ray const& world_ray,
          std::vector<intersection>& world_isecs) noexcept {
    //    std::array<float, 2> isecs;

    for (unsigned sphere_idx = 0; sphere_idx < world.spheres.size; ++sphere_idx) {
        tform4 const inv_tform = inverse(world.spheres.tforms[sphere_idx]);
        ray const object_ray{
            inv_tform * world_ray.origin,
            // do not normalize the result, so we get the t straight for the world space without the
            // need to convert it first from the object space (why convert? because the t we get is
            // for the transformed ray, not for the world space ray). this results in u (see below)
            // not being a unit vector, which results in more calculation. but it is worth it
            inv_tform * world_ray.direction};
        // TODO: test performance without branching

        intersect_sphere(object_ray, shape_id_from_sphere(sphere_idx), world_isecs);

        //        if (intersect_sphere(object_ray, isecs)) {
        //            world_isecs.emplace_back(shape_id_from_sphere(sphere_idx), isecs[0]);
        //            world_isecs.emplace_back(shape_id_from_sphere(sphere_idx), isecs[1]);
        //        }
    }

    // Find first smallest non-negative t, which represents closest intersection.
    std::sort(world_isecs.begin(), world_isecs.end());
    return std::upper_bound(
        world_isecs.cbegin(), world_isecs.cend(), 0.f,
        [](float predicate, intersection const& isec) { return predicate <= isec.t; });
}

unsigned shape_id_from_sphere(unsigned sphere_idx) noexcept {
    // Spheres are the first in the struct, so there is no offset
    return sphere_idx;
}

color shade_hit(world const& world, shading const& shading_info,
                std::vector<intersection>& world_isecs) noexcept {
    return lighting(world.spheres.materials[shading_info.isec.shape_id], world.light,
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
    vec3 const direction = normalize(v);
    ray const r{world_point, direction};

    world_isecs.clear();
    auto hit = intersect(world, r, world_isecs);
    if (hit != world_isecs.cend() && hit->t < dist) {
        return true;
    }
    return false;
}

} // namespace wt
