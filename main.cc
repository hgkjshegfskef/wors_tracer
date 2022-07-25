#include "canvas.hh"
#include "lighting.hh"
#include "log.hh"
#include "pnt3.hh"
#include "pnt_light.hh"
#include "ray.hh"
#include "sphere.hh"
#include "util.hh" // lerp

#include <fmt/chrono.h>
#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include <chrono> // steady_clock

using namespace wt;

int main(int argc, char** argv) {
    (void)argc;
    setup_logging();

    // Take a ray, and a sphere, and a canvas behind the sphere. Then penetrate sphere with the ray
    // and cast its silhouette on the canvas.

    // Where ray starts.
    pnt3 ray_origin{0, 0, -5};

    // Where canvas is positioned.
    float wall_z = 10;

    // How large the wall will be. Because we are using unit spheres, the max y value will be 1. To
    // find out the value, we take the ray origin and the highest point on the sphere and
    // extrapolate between them. Then, we need a value twice as big as the result (for +y and -y
    // values), plus some small margin.
    float wall_size = lerp(wall_z, {ray_origin.z, ray_origin.y}, {0., 1.}) * 2 + 1;
    SPDLOG_DEBUG("Wall size: {}", wall_size);

    // How large canvas is in pixels.
    unsigned canvas_pixels = 1000;
    SPDLOG_TRACE("canvas size: {}x{} pixels", canvas_pixels, canvas_pixels);

    // Size of single pixel (in world space units).
    float pixel_size = wall_size / canvas_pixels;
    SPDLOG_TRACE("pixel size: {}", pixel_size);

    // Half of the wall, which describes minimum and maximum x and y coordinates of the wall.
    float half = wall_size / 2.;

    canvas canvas{canvas_pixels, canvas_pixels};
    canvas.fill({0, 0, 0});
//    color red{1, 0, 0};

    //    tform4 center{1.f, 0.f, 0.f, 1.f, //
    //                  0.f, 1.f, 0.f, 1.f, //
    //                  0.f, 0.f, 1.f, 0.f};
    sphere const s{tform4{}, {.col{1.f, 0.2f, 1.f}}};
//    sphere const s;
    pnt3 const s_center{0.f, 0.f, 0.f};
    tform4 const s_tform_inv = inverse(s.tform);

    // Light setup
    pnt3 const light_position{-10.f, 10.f, -10.f}; // above and to the left of eye
    color const light_intensity{1.f, 1.f, 1.f};
    pnt_light const light{light_position, light_intensity};

    pnt3 wall_point{{}, {}, wall_z};
    ray world_r{ray_origin, {}};
    ray object_r{s_tform_inv * world_r.origin, {}};

    auto const start = std::chrono::steady_clock::now();

    // Start from higher Y to process the canvas memory sequentially.
    for (int y = canvas_pixels - 1; y >= 0; --y) {
        wall_point.y = half - pixel_size * y;
        for (unsigned x = 0; x < canvas_pixels; ++x) {
            wall_point.x = half - pixel_size * x;
            world_r.direction = normalize(wall_point - ray_origin);
            object_r.direction = normalize(s_tform_inv * world_r.direction);

            if (auto const t = intersect_sphere(object_r)) {
                SPDLOG_TRACE("Hit at ({},{})", x, y);
                pnt3 const pos_on_object_r = position(object_r.origin, object_r.direction, *t);
                pnt3 const pos_on_world_r = s.tform * pos_on_object_r;
                canvas(x, y) =
                    lighting(s.mat, light, pos_on_world_r, -world_r.direction,
                             normalize(normalize(pos_on_object_r - s_center) * s_tform_inv));
            }
        }
    }

    auto const stop = std::chrono::steady_clock::now();
    std::chrono::duration<double> const time_s = stop - start;
    SPDLOG_INFO("Raytracing {}x{} pixels took {} ({})", canvas_pixels, canvas_pixels,
                std::chrono::duration<double>(time_s),
                std::chrono::duration<double, std::milli>(time_s));

    write_ppm(argv[1], canvas.as_ppm());
}
