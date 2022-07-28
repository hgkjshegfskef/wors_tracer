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
#include "sphere_operations.hh"
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

std::vector<intersection> intersect(ray const& world_r, world const& w) noexcept {
    std::vector<intersection> world_isecs;
    world_isecs.reserve(w.shapes.size() * 2);
    for (auto const& s : w.shapes) {
        auto const isecs = intersect(s, world_r);
        world_isecs.insert(world_isecs.end(), isecs.begin(), isecs.end());
    }
    return world_isecs;
}

color shade_hit(world const& w, shading const& sh) noexcept {
    return lighting(get_mat(*sh.isec.s), w.light, sh.isec_pnt, sh.eye, sh.normal,
                    is_shadowed(w, sh.over_pnt));
}

color color_at(world const& w, ray const& r) noexcept {
    auto isecs = intersect(r, w);
    //    std::sort(isecs.begin(), isecs.end());
    //    auto isec_it =
    //        std::find_if(isecs.begin(), isecs.end(), [](auto const& isec) { return isec.t > 0; });
    //    if (isec_it != isecs.end()) {
    //        shading sh{*isec_it, r};
    //        return shade_hit(w, sh);
    //    }
    intersection const isec = hit(isecs);
    if (isec.empty()) {
        return color{0, 0, 0};
    }
    shading sh{isec, r};
    return shade_hit(w, sh);
}

bool is_shadowed(world const& w, pnt3 const& p) noexcept {
    vec3 v = w.light.position - p;
    float dist = magnitude(v);
    vec3 direction = normalize(v);
    ray r{p, direction};
    auto isecs = intersect(r, w);
    intersection isec = hit(isecs);
    if (!isec.empty() && isec.t < dist) {
        return true;
    }
    return false;
}

} // namespace wt
