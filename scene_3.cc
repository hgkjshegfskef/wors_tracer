#include "cli.hh"
#include "mat3.hh"
#include "plane.hh"
#include "scene.hh"
#include "shape.hh"
#include "sphere.hh"
#include "util.hh"

#include <numbers>
#include <vector>

using namespace std::numbers;

namespace wt {

scene scene_3(cli const& cli) noexcept {
    tform4 vertical = tform4::translate({0, 0, 5}) * rotation<Axis::X>(deg_to_rad(90));

    plane floor;
    plane back_wall{vertical};
    plane hex_1{rotation<Axis::Y>(deg_to_rad(1 * 60)) * vertical};
    plane hex_2{rotation<Axis::Y>(deg_to_rad(2 * 60)) * vertical};
    plane hex_3{rotation<Axis::Y>(deg_to_rad(3 * 60)) * vertical};
    plane hex_4{rotation<Axis::Y>(deg_to_rad(4 * 60)) * vertical};
    plane hex_5{rotation<Axis::Y>(deg_to_rad(5 * 60)) * vertical};

    sphere middle{tform4::translate({0, 2.f, 0}) * scale(2, 2, 2)};

    std::vector<shape> shapes;
    shapes.reserve(10);
    for (auto& plane : {floor, back_wall, hex_1, hex_2, hex_3, hex_4, hex_5}) {
        shapes.emplace_back(plane);
    }
    for (auto& sphere : {middle}) {
        shapes.emplace_back(sphere);
    }

    world world{std::move(shapes), pnt_light{pnt3{-5, 5, 0}, color{1, 1, 1}}};
    look_at look_at{
        {5.7053213, 20.258598, 0.012088779}, {5.3586664, 19.322538, -0.04810056}, {0, 1, 0}};
    camera camera{cli.tex_width, cli.tex_height, pi_v<float> / 3, look_at};

    return {std::move(world), std::move(camera), std::move(look_at)};
}

} // namespace wt
