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
    sphere middle{tform4::translate({0, 1.f, 0})};

    std::vector<shape> shapes;
    shapes.reserve(6);
    for (auto& plane : {floor}) {
        shapes.emplace_back(plane);
    }
    for (auto& sphere : {middle}) {
        shapes.emplace_back(sphere);
    }

    world world{std::move(shapes), pnt_light{pnt3{-10, 10, -10}, color{1, 1, 1}}};

    camera camera{cli.tex_width, cli.tex_height, pi_v<float> / 3};
    pnt3 from{0, 1.5f, -5};
    pnt3 to{0, 1, 0};
    vec3 up{0, 1, 0};

    if (cli.render_backend == "ppm") {
        camera.inv_tform = inverse(v2::view(from, to, up));
    } else {
        camera.inv_tform = inverse(view(from, to, up));
    }

    return {std::move(world), std::move(camera)};
}

} // namespace wt
