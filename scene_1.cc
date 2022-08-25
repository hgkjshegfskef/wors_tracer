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

scene scene_1(cli const& cli) noexcept {
    tform4 rot_1{rotation<Axis::Y>(-pi_v<float> / 4)};
    tform4 rot_2{rotation<Axis::Y>(pi_v<float> / 4)};
    tform4 rot_3{rotation<Axis::X>(pi_v<float> / 2)};
    tform4 scal{scale(10, .01f, 10)};

    sphere floor{scal, material{.col = {1, .9f, .9f}, .specular = 0}};
    sphere left_wall{tform4::translate({0, 0, 5}) * rot_1 * rot_3 * scal, floor.material};
    sphere right_wall{tform4::translate({0, 0, 5}) * rot_2 * rot_3 * scal, floor.material};
    sphere middle{tform4::translate({-0.5f, 1, .5f})};
    sphere right{tform4::translate({1.5f, .5f, -0.5f}) * scale(.5f, .5f, .5f),
                 material{.col = {.5f, 1, .1f}, .diffuse = .7f, .specular = .3f}};
    sphere left{tform4::translate({-1.5f, .33f, -0.75f}) * scale(.33f, .33f, .33f),
                material{.col = {1, .8f, .1f}, .diffuse = .7f, .specular = .3f}};

    std::vector<shape> shapes;
    shapes.reserve(6);
    for (auto& sphere : {floor, left_wall, right_wall, middle, right, left}) {
        shapes.emplace_back(sphere);
    }

    world world{std::move(shapes), pnt_light{pnt3{-10, 10, -10}, color{1, 1, 1}}};
    look_at look_at{{0, 1.5f, -5}, {0, 1, 0}, {0, 1, 0}};

    camera camera;
    if (cli.render_backend == "ppm") {
        camera = {cli.tex_width, cli.tex_height, pi_v<float> / 3, look_at, true};
    } else {
        camera = {cli.tex_width, cli.tex_height, pi_v<float> / 3, look_at, false};
    }

    return {std::move(world), std::move(camera), std::move(look_at)};
}

} // namespace wt
