#include "checker2d_pattern.hh"
#include "cli.hh"
#include "mat3.hh"
#include "plane.hh"
#include "scene.hh"
#include "shape.hh"
#include "solid_pattern.hh"
#include "sphere.hh"

#include <spdlog/spdlog.h>

#include <numbers>
#include <vector>

using namespace std::numbers;

namespace wt {

scene scene_10(cli const& cli) noexcept {
    std::vector<shape> shapes;

    plane bottom_floor{{},
                       material{.pattern = checker2d_pattern{solid_pattern{{.5, .5, .5}},
                                                             solid_pattern{{.3, .3, .3}}},
                                .ambient = .9,
                                .specular = .1}};
    shapes.emplace_back(bottom_floor);

    color yellow = {.9, .9, 0};
    color white = {1, 1, 1};
    color dark_red = {.1, 0, 0};

    sphere sphere1{tform4::translate({1.2, .5, 1}) * scale(.5, .5, .5),
                   material{.col = yellow,
                            .ambient = .1,
                            .diffuse = .1,
                            .specular = 1,
                            .shininess = 300,
                            .reflectivity = 1,
                            .transparency = 1,
                            .refraction_index = 1.5}};
    shapes.emplace_back(sphere1);

    // white glass
    sphere sphere2{tform4::translate({0, .5, 0}) * scale(.5, .5, .5),
                   material{.col = white,
                            .ambient = .1,
                            .diffuse = .1,
                            .specular = 1,
                            .shininess = 300,
                            .reflectivity = 1,
                            .transparency = 1,
                            .refraction_index = 1.5}};
    shapes.emplace_back(sphere2);

    // red glass
    sphere sphere3{tform4::translate({-1.2, .5, 1.2}) * scale(.5, .5, .5),
                   material{.col = dark_red,
                            .ambient = .9,
                            .diffuse = .1,
                            .specular = 1,
                            .shininess = 300,
                            .reflectivity = 1,
                            .transparency = 1,
                            .refraction_index = 1.5}};
    shapes.emplace_back(sphere3);

    world world{std::move(shapes), pnt_light{pnt3{-4, 4, -4}, color{1, 1, 1}}};
    look_at look_at{{0, 2, -5}, {0, 1, 0}, {0, 1, 0}};
    camera camera{cli.tex_width, cli.tex_height, pi_v<float> / 3, look_at};

    return {std::move(world), std::move(camera), std::move(look_at)};
}

} // namespace wt
