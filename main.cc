#include "camera.hh"
#include "canvas.hh"
#include "log.hh"
#include "mat3.hh"
#include "pnt_light.hh"
#include "render.hh"
#include "shape.hh"
#include "sphere.hh"
#include "util.hh" // write_ppm
#include "world.hh"

#include <fmt/chrono.h>
#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include <chrono>  // steady_clock
#include <numbers> // pi_v

using namespace wt;
using namespace std::numbers;

int main(int argc, char** argv) {
    (void)argc;
    setup_logging();

    tform4 rot_1{rotation<Axis::Y>(-pi_v<float> / 4)};
    tform4 rot_2{rotation<Axis::Y>(pi_v<float> / 4)};
    tform4 rot_3{rotation<Axis::X>(pi_v<float> / 2)};
    tform4 scal{scale(10, .01f, 10)};

    sphere floor{scal, material{.col = {1, .9f, .9f}, .specular = 0}};
    sphere left_wall{tform4::translate({0, 0, 5}) * rot_1 * rot_3 * scal, floor.mat};
    sphere right_wall{tform4::translate({0, 0, 5}) * rot_2 * rot_3 * scal, floor.mat};
    sphere middle{tform4::translate({-0.5f, 1, .5f}),
                  material{.col = {.1f, 1, .5f}, .diffuse = .7f, .specular = .3f}};
    sphere right{tform4::translate({1.5f, .5f, -0.5f}) * scale(.5f, .5f, .5f),
                 material{.col = {.5f, 1, .1f}, .diffuse = .7f, .specular = .3f}};
    sphere left{tform4::translate({-1.5f, .33f, -0.75f}) * scale(.33f, .33f, .33f),
                material{.col = {1, .8f, .1f}, .diffuse = .7f, .specular = .3f}};

    std::vector<shape> shapes;
    shapes.emplace_back(floor);
    shapes.emplace_back(left_wall);
    shapes.emplace_back(right_wall);
    shapes.emplace_back(middle);
    shapes.emplace_back(right);
    shapes.emplace_back(left);

    world w{pnt_light{pnt3{-10, 10, -10}, color{1, 1, 1}}, shapes};

    camera cam{1920, 1080, pi_v<float> / 3};
    pnt3 from{0, 1.5f, -5};
    pnt3 to{0, 1, 0};
    vec3 up{0, 1, 0};
    cam.tform = v2::view(from, to, up);

    auto const start = std::chrono::steady_clock::now();

    canvas image = render(cam, w);

    auto const stop = std::chrono::steady_clock::now();
    std::chrono::duration<double> const time_s = stop - start;
    SPDLOG_INFO("Raytracing {}x{} pixels took {} ({})", cam.hsize, cam.vsize,
                std::chrono::duration<double>(time_s),
                std::chrono::duration<double, std::milli>(time_s));

    write_ppm(argv[1], image.as_ppm());
}
