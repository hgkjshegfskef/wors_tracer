#include "world.hh"

#include "ray.hh"

#include <algorithm> // sort
#include <utility>   // move

namespace wt {

world::world(pnt_light light, std::vector<sphere> spheres) noexcept
    : light{std::move(light)}, shapes{std::move(spheres)} {}

world world::make_default() noexcept {
    return world{
        pnt_light{pnt3{-10.f, 10.f, -10.f}, color{1.f, 1.f, 1.f}},
        std::vector{sphere{tform4{},
                           material{.col = color{.8f, 1.f, .6f}, .diffuse = .7f, .specular = .2f}},
                    sphere{tform4{scale(.5f, .5f, .5f)}}}};
}

std::vector<float> intersect(ray const& world_r, world const& w) noexcept {
    std::vector<float> ts;
    for (auto const& s : w.shapes) {
        tform4 const inv_tform = inverse(s.tform);
        ray const object_r{inv_tform * world_r.origin, inv_tform * world_r.direction};
        if (auto t = intersect_sphere(world_r)) {
            ts.push_back(*t);
        }
    }
    std::sort(ts.begin(), ts.end());
    return ts;
}

} // namespace wt
