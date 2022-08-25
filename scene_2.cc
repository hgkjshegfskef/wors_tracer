#include "cli.hh"
#include "mat3.hh"
#include "plane.hh"
#include "scene.hh"
#include "shape.hh"
#include "sphere.hh"

#include <numbers>
#include <vector>

using namespace std::numbers;

namespace wt {

scene scene_2(cli const& cli) noexcept {
    plane floor;
    plane back_wall{tform4::translate({0, 0, 5}) * rotation<Axis::X>(pi_v<float> / 2)};

    sphere middle{tform4::translate({0, 1.f, 0})};

    std::vector<shape> shapes;
    shapes.reserve(10);
    for (auto& plane : {floor, back_wall}) {
        shapes.emplace_back(plane);
    }
    for (auto& sphere : {middle}) {
        shapes.emplace_back(sphere);
    }

    world world{std::move(shapes), pnt_light{pnt3{-10, 10, -10}, color{1, 1, 1}}};
    look_at look_at{{0, 1, -5}, {0, 1, 0}, {0, 1, 0}};

    camera camera;
    if (cli.render_backend == "ppm") {
        camera = {cli.tex_width, cli.tex_height, pi_v<float> / 3, look_at, true};
    } else {
        camera = {cli.tex_width, cli.tex_height, pi_v<float> / 3, look_at, false};
    }

    return {std::move(world), std::move(camera), std::move(look_at)};
}

} // namespace wt
