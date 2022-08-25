#include "checker2d_pattern.hh"
#include "checker3d_pattern.hh"
#include "cli.hh"
#include "gradient_pattern.hh"
#include "mat3.hh"
#include "pattern.hh"
#include "plane.hh"
#include "radial_gradient_pattern.hh"
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

scene scene_5(cli const& cli) noexcept {
    tform4 rot_1{rotation<Axis::Y>(-pi_v<float> / 4)};
    tform4 rot_2{rotation<Axis::Y>(pi_v<float> / 4)};
    tform4 rot_3{rotation<Axis::X>(pi_v<float> / 2)};

    plane floor{
        {}, material{.pattern = pattern{stripe_pattern{{1, 1, 1}, {.8, .8, .8}, scale(2, 2, 2)}}}};
    plane left_wall{tform4::translate({0, 0, 5}) * rot_1 * rot_3,
                    material{.pattern = pattern{gradient_pattern{
                                 {.7, 0, 0}, {1, 1, .3}, rotation<Axis::Y>(pi_v<float> / 2)}}}};
    plane right_wall{tform4::translate({0, 0, 5}) * rot_2 * rot_3,
                     material{.pattern = pattern{checker2d_pattern{{1, 0, 0}, {.5, .5, 0}}}}};

    sphere left{tform4::translate({-2, 1, 0})
                //            * scale(.33f, .33f, .33f)
                ,
                material{.pattern = pattern{ring_pattern{
                             {.7, 0, 0}, {1, 1, .3},
                             //                                                 scale(.33, .33, .33)
                         }},
                         .diffuse = .7f,
                         .specular = .3f}};
    sphere center{tform4::translate({0, 1, 0})
                  //                           * scale(.5, .5, .5)
                  ,
                  material{.pattern = pattern{radial_gradient_pattern{
                               {.7, 0, 0}, {1, 1, .3},
                               //                                    scale(.5, .5, .5)
                           }},
                           .diffuse = .7f,
                           .specular = .3f}};

    sphere right{
        tform4::translate({2, 1, 0})
        //            * scale(.5, .5, .5)
        ,
        material{.pattern = pattern{gradient_pattern{
                     {.7, 0, 0}, {1, 1, .3},
                     //                                                     scale(.5, .5, .5)
                 }},
                 .diffuse = .7f,
                 .specular = .3f}};

    std::vector<shape> shapes;
    shapes.reserve(8);
    for (auto& sphere : {left, center, right}) {
        shapes.emplace_back(sphere);
    }
    for (auto& plane : {floor, left_wall, right_wall}) {
        shapes.emplace_back(plane);
    }

    world world{std::move(shapes), pnt_light{pnt3{-10, 10, -10}, color{1, 1, 1}}};
    look_at look_at{{0, 2, -7}, {0, 1, 0}, {0, 1, 0}};
    camera camera{cli.tex_width, cli.tex_height, pi_v<float> / 3, look_at};

    return {std::move(world), std::move(camera), std::move(look_at)};
}

} // namespace wt
