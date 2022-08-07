#include "cli.hh"
#include "mat3.hh"
#include "scene.hh"
#include "sphere.hh"

#include <cassert>
#include <numbers>
#include <vector>

using namespace std::numbers;

namespace wt {

scene scene_0(cli const& cli) noexcept {
    sphere s{};

    struct spheres spheres(1);
    assert(spheres.add_sphere(std::move(s)));

    world world{std::move(spheres), pnt_light{pnt3{-10, 10, -10}, color{1, 1, 1}}};

    camera camera{cli.tex_width, cli.tex_height, pi_v<float> / 3};
    pnt3 from{0, 0, -2};
    pnt3 to{0, 0, 0};
    vec3 up{0, 1, 0};

    if (cli.render_backend == "ppm") {
        camera.tform = v2::view(from, to, up);
    } else {
        camera.tform = view(from, to, up);
    }

    return {std::move(world), std::move(camera)};
}

} // namespace wt
