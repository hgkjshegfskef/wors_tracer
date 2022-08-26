#include "checker2d_pattern.hh"
#include "cli.hh"
#include "mat3.hh"
#include "plane.hh"
#include "scene.hh"
#include "solid_pattern.hh"
#include "stripe_pattern.hh"
#include "util.hh"

#include <numbers>
#include <vector>

using namespace std::numbers;

namespace wt {

scene scene_6(cli const& cli) noexcept {
    plane floor{
        {},
        material{.pattern = checker2d_pattern{
                     stripe_pattern{solid_pattern{0x77283B}, solid_pattern{0x964B5E},
                                    rotation<Axis::Y>(deg_to_rad(45)) * scale(.1, .1, .1)},
                     stripe_pattern{solid_pattern{0x656565}, solid_pattern{0x3C3C3C},
                                    rotation<Axis::Y>(deg_to_rad(-45)) * scale(.1, .1, .1)}}}};

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
