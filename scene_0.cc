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

    camera camera{cli.tex_width, cli.tex_height, pi_v<float> / 3};
    pnt3 from{0, 0, -2};
    pnt3 to{0, 0, 0};
    vec3 up{0, 1, 0};

    if (cli.render_backend == "ppm") {
        camera.inv_tform = inverse(v2::view(from, to, up));
    } else {
        camera.inv_tform = inverse(view(from, to, up));
    }

    return {std::move(world), std::move(camera)};
}

} // namespace wt