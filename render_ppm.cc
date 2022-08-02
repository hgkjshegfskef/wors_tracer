#include "camera.hh"
#include "canvas.hh"
#include "color.hh"
#include "intersection.hh"
#include "ray.hh"
#include "render.hh"
#include "shape.hh"
#include "tform4.hh"
#include "util.hh"
#include "world.hh"

#include <oneapi/tbb/blocked_range2d.h>
#include <oneapi/tbb/parallel_for.h>

#include <fmt/chrono.h>
#include <spdlog/spdlog.h>

#include <chrono>

using namespace oneapi;

namespace wt {

void render_ppm(camera const& camera, world const& world) noexcept {
    canvas image{camera.hsize, camera.vsize};
    //    image.fill({0, 0, 0});
    tform4 const inv_cam_tform = inverse(camera.tform);

    unsigned frames = 1;
    for (unsigned frame = 0; frame < frames; ++frame) {
        auto start = std::chrono::steady_clock::now();

        tbb::parallel_for(
            tbb::blocked_range2d<int>(0, camera.vsize, 0, camera.hsize), [&](auto const range) {
                std::vector<intersection> world_isecs;
                world_isecs.reserve(world.shapes.size() * 2);
                for (int y = range.rows().begin(); y != range.rows().end(); ++y) {
                    for (int x = range.cols().begin(); x != range.cols().end(); ++x) {
                        image(x, y) = color_at(world, ray_for_pixel(camera, inv_cam_tform, x, y),
                                               world_isecs);
                    }
                }
            });

        auto stop = std::chrono::steady_clock::now();

        SPDLOG_INFO("Frame #{} drawn in {}", frame,
                    std::chrono::duration<double, std::milli>(stop - start));
    }

    write_ppm("image.ppm", image.as_ppm());
}

} // namespace wt
