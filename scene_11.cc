#include "blended_pattern.hh"
#include "checker2d_pattern.hh"
#include "checker3d_pattern.hh"
#include "cli.hh"
#include "cube.hh"
#include "gradient_pattern.hh"
#include "mat3.hh"
#include "perturb_pattern.hh"
#include "ring_pattern.hh"
#include "scene.hh"
#include "solid_pattern.hh"
#include "stripe_pattern.hh"
#include "util.hh"

#include <numbers>

using namespace std::numbers;

namespace wt {

scene scene_11(cli const& cli) noexcept {
    color white{1, 1, 1};
    color light_wood{.6, .4, 0};
    color dark_wood{.3, .2, 0};
    color light_wood_1{.65, .45, 0};
    color dark_wood_1{.35, .25, 0};
    color dark_red = {.1, 0, 0};

    std::vector<shape> shapes;

    shape cube1{cube{
        scale(6 - 0.1, 2, 4 - 0.1),
        {
            .pattern =
                perturb_pattern{
                    stripe_pattern{
                        solid_pattern{light_wood},
                        solid_pattern{dark_wood},
                        rotation<Axis::Z>(deg_to_rad(90)) * scale(.1, .1, .1),
                    },
                    .1,
                    3,
                },
        },
    }};
    shapes.emplace_back(cube1);

    shape cube2{cube{
        scale(6, 2 - 0.1, 4),
        {
            .pattern =
                perturb_pattern{
                    ring_pattern{
                        ring_pattern{
                            solid_pattern{light_wood},
                            solid_pattern{dark_wood},
                            scale(.1, .1, .1),
                        },
                        ring_pattern{
                            solid_pattern{light_wood_1},
                            solid_pattern{dark_wood_1},
                            scale(.1, .1, .1),
                        },
                        scale(.5, .5, .5),
                    },
                    1,
                    4,
                },
        },
    }};
    shapes.emplace_back(cube2);

    shape mirror{cube{
        tform4::translate({-1, 0, 3.88}) * rotation<Axis::X>(deg_to_rad(90)) * scale(1.5, .01, 1),
        {
            .col = white,
            .ambient = 0,
            .diffuse = .1,
            .reflectivity = .9,
            .refraction_index = 1.5,
        },
    }};
    shapes.emplace_back(mirror);

    shape mirror_frame{cube{
        tform4::translate({-1, 0, 3.89}) * rotation<Axis::X>(deg_to_rad(90)) * scale(1.7, .01, 1.2),
        {
            .col = dark_wood_1,
        },
    }};
    shapes.emplace_back(mirror_frame);

    shape table_top{cube{
        tform4::translate({-1.5, -0.5, .5}) * scale(1.7, .03, 1.2),
        {
            .pattern =
                perturb_pattern{
                    checker3d_pattern{
                        solid_pattern{light_wood},
                        solid_pattern{dark_wood},
                        scale(.1, .1, .1),
                    },
                    1,
                    8,
                },
        },
    }};
    shapes.emplace_back(table_top);

    shape table_leg_1{cube{
        tform4::translate({-3, -1.25, 1.5}) * rotation<Axis::Y>(90) * scale(.05, .75, .05),
        {
            .pattern =
                gradient_pattern{
                    solid_pattern{light_wood},
                    solid_pattern{0, 0, 0},
                    tform4::translate({0, 1.4, 0}) * rotation<Axis::Z>(90) * scale(2.5, 2.5, 2.5),
                },
        },
    }};
    shapes.emplace_back(table_leg_1);

    shape table_leg_2{cube{
        tform4::translate({-3, -1.25, -0.5}) * rotation<Axis::Y>(90) * scale(.05, .75, .05),
        {
            .pattern =
                gradient_pattern{
                    solid_pattern{light_wood},
                    solid_pattern{0, 0, 0},
                    tform4::translate({0, 1.4, 0}) * rotation<Axis::Z>(90) * scale(2.5, 2.5, 2.5),
                },
        },
    }};
    shapes.emplace_back(table_leg_2);

    shape table_leg_3{cube{
        tform4::translate({0, -1.25, -0.5}) * rotation<Axis::Y>(90) * scale(.05, .75, .05),
        {
            .pattern =
                gradient_pattern{
                    solid_pattern{light_wood},
                    solid_pattern{0, 0, 0},
                    tform4::translate({0, 1.4, 0}) * rotation<Axis::Z>(90) * scale(2.5, 2.5, 2.5),
                },
        },
    }};
    shapes.emplace_back(table_leg_3);

    shape table_leg_4{cube{
        tform4::translate({0, -1.25, 1.5}) * rotation<Axis::Y>(90) * scale(.05, .75, .05),
        {
            .pattern =
                gradient_pattern{
                    solid_pattern{light_wood},
                    solid_pattern{0, 0, 0},
                    tform4::translate({0, 1.4, 0}) * rotation<Axis::Z>(90) * scale(2.5, 2.5, 2.5),
                },
        },
    }};
    shapes.emplace_back(table_leg_4);

    shape cube_on_top{cube{
        tform4::translate({-1.5, -0.15, .5}) * scale(.3, .3, .3),
        {
            .col = dark_red,
            .reflectivity = 0.2,
            .transparency = 0.8,
            .refraction_index = 1.5,
        },
        false,
    }};
    shapes.emplace_back(cube_on_top);

    world world{std::move(shapes), pnt_light{pnt3{-1, 1.5, -1.5}, white}};
    look_at look_at{
        {2.2290604, 1.2491266, -3.542432}, {1.6316772, 0.9751672, -2.7887216}, {0, 1, 0}};
    camera camera{cli.tex_width, cli.tex_height, pi_v<float> / 3, look_at};

    return {std::move(world), std::move(camera), std::move(look_at)};
}

} // namespace wt
