#include "canvas.hh"
#include "log.hh"
#include "pnt3.hh"
#include "ray.hh"
#include "sphere.hh"
#include "util.hh" // lerp

#include <fmt/chrono.h>
#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include <chrono> // steady_clock

using namespace wt;

int main(int argc, char** argv) {
    setup_logging();

    // Take a ray, and a sphere, and a canvas behind the sphere. Then penetrate sphere with the ray
    // and cast its silhouette on the canvas.

    // Where ray starts.
    pnt3 ray_origin{0, 0, 5};

    // Where canvas is positioned.
    float wall_z = -10;

    // How large the wall will be. Because we are using unit spheres, the max y value will be 1. To
    // find out the value, we take the ray origin and the highest point on the sphere and
    // extrapolate between them. Then, we need a value twice as big as the result (for +y and -y
    // values), plus some small margin.
    float wall_size = lerp(wall_z, {ray_origin.z, ray_origin.y}, {0., 1.}) * 2 + 1;
    SPDLOG_DEBUG("Wall size: {}", wall_size);

    // How large canvas is in pixels.
    unsigned canvas_pixels = 100;

    // Size of single pixel (in world space units).
    float pixel_size = wall_size / canvas_pixels;

    // Half of the wall, which describes minimum and maximum x and y coordinates of the wall.
    float half = wall_size / 2.;

    canvas canvas{canvas_pixels, canvas_pixels};
    canvas.fill({0, 0, 0});
    color red{1, 0, 0};

    sphere s;

    auto start = std::chrono::steady_clock::now();

    for (unsigned y = 0; y < canvas_pixels; ++y) {
        float const world_y = half - pixel_size * y;
        for (unsigned x = 0; x < canvas_pixels; ++x) {
            float const world_x = half - pixel_size * x;
            pnt3 const wall_point{world_x, world_y, wall_z};
            ray const r{ray_origin, normalize(wall_point - ray_origin)};
            if (auto const isec = intersect(r, s)) {
                SPDLOG_TRACE("Hit at ({},{})", x, y);
                canvas(x, y) = red;
            }
        }
    }

    //    float x = 4;
    //    float y = 0;
    //    pnt3 wall_point{float(x), float(y), wall_z};
    //    ray r{ray_origin, normalize(wall_point - ray_origin)};
    //    if (auto isec = intersect(r, shape)) {
    //        SPDLOG_TRACE("Hit at ({},{})", x, y);
    //        canvas(x, y) = red;
    //    }

    auto stop = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> time_ms = stop - start;
    SPDLOG_INFO("Raytracing {}x{} pixels took {} ({})", canvas_pixels, canvas_pixels,
                std::chrono::duration<double>(stop - start),
                std::chrono::duration<double, std::milli>(stop - start));

    write_ppm(argv[1], canvas.as_ppm());
}
