#include "cli.hh"
#include "mat3.hh"
#include "scene.hh"
#include "shape.hh"
#include "sphere.hh"

#include <numbers>
#include <vector>

using namespace std::numbers;

namespace wt {

scene scene_1(cli const& cli) noexcept {
    tform4 rot_1{rotation<Axis::Y>(-pi_v<float> / 4)};
    tform4 rot_2{rotation<Axis::Y>(pi_v<float> / 4)};
    tform4 rot_3{rotation<Axis::X>(pi_v<float> / 2)};
    tform4 scal{scale(10, .01f, 10)};

    sphere floor{scal, material{.col = {1, .9f, .9f}, .specular = 0}};
    sphere left_wall{tform4::translate({0, 0, 5}) * rot_1 * rot_3 * scal, floor.mat};
    sphere right_wall{tform4::translate({0, 0, 5}) * rot_2 * rot_3 * scal, floor.mat};
    sphere middle{tform4::translate({-0.5f, 1, .5f})};
    sphere right{tform4::translate({1.5f, .5f, -0.5f}) * scale(.5f, .5f, .5f),
                 material{.col = {.5f, 1, .1f}, .diffuse = .7f, .specular = .3f}};
    sphere left{tform4::translate({-1.5f, .33f, -0.75f}) * scale(.33f, .33f, .33f),
                material{.col = {1, .8f, .1f}, .diffuse = .7f, .specular = .3f}};

    std::vector<shape> shapes;
    shapes.emplace_back(std::move(floor));
    shapes.emplace_back(std::move(left_wall));
    shapes.emplace_back(std::move(right_wall));
    shapes.emplace_back(std::move(middle));
    shapes.emplace_back(std::move(right));
    shapes.emplace_back(std::move(left));

    world const world{pnt_light{pnt3{-10, 10, -10}, color{1, 1, 1}}, std::move(shapes)};

    camera camera{cli.tex_width, cli.tex_height, pi_v<float> / 3};
    pnt3 from{0, 1.5f, -5};
    pnt3 to{0, 1, 0};
    vec3 up{0, 1, 0};

    if (cli.render_backend == "ppm") {
        camera.tform = v2::view(from, to, up);
    } else {
        camera.tform = view(from, to, up);
    }

    return {world, camera};
}

} // namespace wt
