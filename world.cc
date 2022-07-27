#include "world.hh"

#include "color.hh"
#include "intersection.hh"
#include "lighting.hh"
#include "mat3.hh"
#include "ray.hh"
#include "shading.hh"

#include <algorithm> // sort

namespace wt {

world::world(pnt_light const& light, std::vector<sphere> const& spheres) noexcept
    : light{light}, shapes{spheres} {}

world world::make_default() noexcept {
    return world{
        pnt_light{pnt3{-10.f, 10.f, -10.f}, color{1.f, 1.f, 1.f}},
        std::vector{sphere{tform4{},
                           material{.col = color{.8f, 1.f, .6f}, .diffuse = .7f, .specular = .2f}},
                    sphere{tform4{scale(.5f, .5f, .5f)}}}};
}

std::vector<intersection> intersect(ray const& world_r, world const& w) noexcept {
    std::vector<intersection> world_isecs;
    world_isecs.reserve(w.shapes.size() * 2);
    for (auto const& s : w.shapes) {
        auto const isecs = v2::intersect(world_r, s);
        world_isecs.insert(world_isecs.end(), isecs.begin(), isecs.end());
    }
    return world_isecs;
}

color shade_hit(world const& w, shading const& sh) noexcept {
    return lighting(sh.isec.s->mat, w.light, sh.isec_pnt, sh.eye, sh.normal);
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
    intersection const isec = v2::hit(isecs);
    if (isec.empty()) {
        return color{0, 0, 0};
    }
    shading sh{isec, r};
    return shade_hit(w, sh);
}

} // namespace wt
