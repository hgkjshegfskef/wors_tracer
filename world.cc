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
#include "util.hh"

#include <algorithm> // sort
#include <cmath>     // sqrt
#include <utility>
#include <vector>

using std::placeholders::_2;
using std::placeholders::_5;

namespace wt {

world::world() noexcept = default;

world::world(std::vector<shape> shapes, pnt_light light) noexcept
    : // this bug was very difficult to fix. I usually use {}-init everywhere, but in c++11+ there
      // is an idiotic rule that initializer_list is preferred even against copy and move ctors, so
      // what it was doing was trying to init every shape with vector...
      shapes(std::move(shapes)), light{std::move(light)} {}

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
                std::vector<intersection>& world_isecs, unsigned remaining) noexcept {
    material const& mat = mater(world.shapes[shading_info.isec.shape_id]);
    bool const in_shadow = is_shadowed(world, shading_info.over_pnt, world_isecs);
    color const surface =
        lighting(mat, world.shapes[shading_info.isec.shape_id], world.light, shading_info.isec_pnt,
                 shading_info.eye, shading_info.normal, in_shadow);
    color const reflected = reflected_color(world, shading_info, world_isecs, remaining);
    color const refracted = refracted_color(world, shading_info, world_isecs, remaining);

    if (mat.reflectivity > 0 && mat.transparency > 0) {
        float const reflectance = schlick(shading_info);
        return surface + reflected * reflectance + refracted * (1 - reflectance);
    }
    return surface + reflected + refracted;
}

color color_at(world const& world, ray const& world_ray, std::vector<intersection>& world_isecs,
               unsigned remaining) noexcept {
    world_isecs.clear();
    auto hit = intersect(world, world_ray, world_isecs);
    if (hit == world_isecs.cend()) {
        return color{0, 0, 0};
    }
    shading const sh{world.shapes, world_ray, world_isecs, *hit};
    color const c = shade_hit(world, sh, world_isecs, remaining);
    return c;
}

bool is_shadowed(world const& world, pnt3 const& world_point,
                 std::vector<intersection>& world_isecs) noexcept {
    vec3 const v = world.light.position - world_point;
    float const dist = magnitude(v);
    world_isecs.clear();
    auto hit = intersect(world, {world_point, normalize(v)}, world_isecs);
    return hit != world_isecs.cend() && cast_shadow(world.shapes[hit->shape_id]) && hit->t < dist;
}

color reflected_color(world const& world, shading const& shading_info,
                      std::vector<intersection>& world_isecs, unsigned remaining) noexcept {
    float const& reflective = mater(world.shapes[shading_info.isec.shape_id]).reflectivity;
    if (reflective == 0 || remaining == 0) {
        return {0, 0, 0};
    }
    ray const reflect{shading_info.over_pnt, shading_info.reflect};
    color const color = color_at(world, reflect, world_isecs, remaining - 1);
    return color * reflective;
}

color refracted_color(world const& world, shading const& shading_info,
                      std::vector<intersection>& world_isecs, unsigned remaining) noexcept {
    if (mater(world.shapes[shading_info.isec.shape_id]).transparency == 0 || remaining == 0) {
        return {0, 0, 0};
    }
    // Snell's Law
    float const n_ratio = shading_info.n1 / shading_info.n2;
    float const cos_i = dot(shading_info.eye, shading_info.normal);
    float const sin2_t = (n_ratio ^ _2) * (1.f - (cos_i ^ _2));
    if (sin2_t > 1.f) {
        // total internal reflection
        return {0, 0, 0};
    }
    float const cos_t = std::sqrt(1.f - sin2_t);
    ray const refract{shading_info.under_pnt,
                      shading_info.normal * (n_ratio * cos_i - cos_t) - shading_info.eye * n_ratio};
    color const c = color_at(world, refract, world_isecs, remaining - 1);
    float const tp = mater(world.shapes[shading_info.isec.shape_id]).transparency;
    return c * tp;
}

float schlick(shading const& sh) noexcept {
    float cos = dot(sh.eye, sh.normal);
    if (sh.n1 > sh.n2) { // maybe total internal reflection
        float const n_ratio = sh.n1 / sh.n2;
        float const sin2_t = (n_ratio ^ _2) * (1.f - (cos ^ _2));
        if (sin2_t > 1.f) { // total internal reflection
            return 1.f;
        }
        float const cos_t = std::sqrt(1.f - sin2_t);
        cos = cos_t;
    }
    float r0 = ((sh.n1 - sh.n2) / (sh.n1 + sh.n2)) ^ _2;
    return r0 + (1 - r0) * ((1 - cos) ^ _5);
}

} // namespace wt
