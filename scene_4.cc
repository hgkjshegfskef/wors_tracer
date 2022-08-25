#include "checker_pattern.hh"
#include "cli.hh"
#include "gradient_pattern.hh"
#include "mat3.hh"
#include "pattern.hh"
#include "plane.hh"
#include "ring_pattern.hh"
#include "scene.hh"
#include "shape.hh"
#include "sphere.hh"
#include "stripe_pattern.hh"

#include <spdlog/spdlog.h>

#include <numbers>
#include <vector>

using namespace std::numbers;

namespace wt {

scene scene_4(cli const& cli) noexcept {
    tform4 rot_1{rotation<Axis::Y>(-pi_v<float> / 4)};
    tform4 rot_2{rotation<Axis::Y>(pi_v<float> / 4)};
    tform4 rot_3{rotation<Axis::X>(pi_v<float> / 2)};

    plane floor{
        {}, material{.pattern = pattern{stripe_pattern{{0, 0, 0}, {1, 1, 1}, {scale(2, 2, 2)}}}}};
    plane left_wall{
        tform4::translate({0, 0, 5}) * rot_1 * rot_3,
        material{.pattern = pattern{gradient_pattern{{1, 1, 0}, {0, 1, 1}, {scale(20, 20, 20)}}}}};
    plane right_wall{tform4::translate({0, 0, 5}) * rot_2 * rot_3,
                     material{.pattern = pattern{ring_pattern{{1, 0, 0}, {.5, .5, 0}}}}};

    sphere middle{
        tform4::translate({-0.5f, 1, .5f}),
        material{.pattern = pattern{gradient_pattern{
                     {1, 0, 1}, {0, 1, 1}, tform4::translate({1, 0, 0}) * scale(2, 1, 1)}}}};
    sphere right{tform4::translate({1.5f, .5f, -0.5f}) * scale(.5f, .5f, .5f),
                 material{.col = {.5f, 1, .1f}, .diffuse = .7f, .specular = .3f}};
    sphere left{tform4::translate({-1.5f, .33f, -0.75f}) * scale(.33f, .33f, .33f),
                material{.col = {1, .8f, .1f}, .diffuse = .7f, .specular = .3f}};

    std::vector<shape> shapes;
    shapes.reserve(6);
    for (auto& sphere : {middle, right, left}) {
        shapes.emplace_back(sphere);
    }
    for (auto& plane : {floor, left_wall, right_wall}) {
        shapes.emplace_back(plane);
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
