#include "blended_pattern.hh"
#include "checker2d_pattern.hh"
#include "cli.hh"
#include "mat3.hh"
#include "perturb_pattern.hh"
#include "plane.hh"
#include "scene.hh"
#include "solid_pattern.hh"
#include "stripe_pattern.hh"
#include "util.hh"

#include <numbers>
#include <vector>

using namespace std::numbers;

namespace wt {

scene scene_8(cli const& cli) noexcept {
    //    plane floor{
    //        {},
    //        material{.pattern = perturb_pattern{
    //                     blended_pattern{
    //                         stripe_pattern{solid_pattern{color{0x1D481D}}, solid_pattern{{1, 1,
    //                         1}}}, stripe_pattern{solid_pattern{color{0x1D481D}},
    //                         solid_pattern{{1, 1, 1}},
    //                                        rotation<Axis::Y>(deg_to_rad(90))},
    //                         rotation<Axis::Y>(deg_to_rad(45))},
    //                     1}}};

    //    plane floor{
    //        {},
    //        material{.pattern =
    //
    //                         checker2d_pattern{
    //                         perturb_pattern{stripe_pattern{solid_pattern{color{0,0,0}},
    //                         solid_pattern{color{1,1,1}},
    //                                        rotation<Axis::Y>(deg_to_rad(45)) * scale(.5, .5,
    //                                        .5)}},
    //                         stripe_pattern{solid_pattern{color{0,1,0}},
    //                         solid_pattern{color{0,0,1}},
    //                                        rotation<Axis::Y>(deg_to_rad(-45)) * scale(.5, .5,
    //                                        .5)}}
    //        }};

    // FIXME: for some reason perlin doesn't apply to patterns rotated 45 degrees, idk why!
    plane floor{{},
                material{.pattern = perturb_pattern{
                             stripe_pattern{solid_pattern{0, 0, 0}, solid_pattern{1, 1, 1},
                                            rotation<Axis::Y>(deg_to_rad(45)) * scale(.5, .5, .5)},
                             1}}};

    //    plane floor{
    //        {},
    //        material{.pattern = perturb_pattern{
    //                     checker2d_pattern{solid_pattern{1, 1, 1}, solid_pattern{0, 0, 0}}, .5}}};

    //    plane floor{
    //        {},
    //        material{
    //            .pattern = perturb_pattern{
    //                checker2d_pattern{
    //                    stripe_pattern{solid_pattern{color{0x77283B}},
    //                    solid_pattern{color{0x964B5E}},
    //                                   rotation<Axis::Y>(deg_to_rad(45)) * scale(.1, .1, .1)},
    //                    stripe_pattern{solid_pattern{color{0x656565}},
    //                    solid_pattern{color{0x3C3C3C}},
    //                                   rotation<Axis::Y>(deg_to_rad(-45)) * scale(.1, .1, .1)}},
    //                .5}}};

    std::vector<shape> shapes;
    for (auto& plane : {floor}) {
        shapes.emplace_back(plane);
    }

    world world{std::move(shapes), pnt_light{pnt3{-10, 10, -10}, color{1, 1, 1}}};
    look_at look_at;
    camera camera{cli.tex_width, cli.tex_height, pi_v<float> / 3, look_at};

    return {std::move(world), std::move(camera), std::move(look_at)};
}

} // namespace wt
