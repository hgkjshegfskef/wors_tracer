#include "cli.hh"
#include "mat3.hh"
#include "scene.hh"
#include "shape.hh"
#include "sphere.hh"

#include <spdlog/spdlog.h>

#include <numbers>
#include <vector>

using namespace std::numbers;

namespace wt {

scene scene_0(cli const& cli) noexcept {
    std::vector<shape> shapes;
    shapes.emplace_back(sphere{});

    world world{std::move(shapes), pnt_light{pnt3{-10, 10, -10}, color{1, 1, 1}}};
    look_at look_at{{0, 0, -2}, {0, 0, 0}, {0, 1, 0}};

    camera camera;
    if (cli.render_backend == "ppm") {
        camera = {cli.tex_width, cli.tex_height, pi_v<float> / 3, look_at, true};
    } else {
        camera = {cli.tex_width, cli.tex_height, pi_v<float> / 3, look_at, false};
    }

    return {std::move(world), std::move(camera), std::move(look_at)};
}

} // namespace wt
