#include "canvas.hh"
#include "cli.hh"
#include "intersection.hh"
#include "ray.hh"
#include "render.hh"
#include "scene.hh"
#include "util.hh"

#include <oneapi/tbb/blocked_range2d.h>
#include <oneapi/tbb/parallel_for.h>

#include <fmt/chrono.h>
#include <spdlog/spdlog.h>

#include <chrono>

using namespace oneapi;

namespace wt {

void render_ppm(cli const& cli, scene const& scene, canvas& image, size_t frames) noexcept {
    for (size_t frame = 0; frame < frames; ++frame) {
        auto start = std::chrono::steady_clock::now();

        tbb::parallel_for(tbb::blocked_range2d<int>(0, scene.camera.vsize, 0, scene.camera.hsize),
                          [&](auto const range) {
                              std::vector<intersection> world_isecs;
                              world_isecs.reserve(scene.world.shapes.size() * 2);
                              for (int y = range.rows().begin(); y != range.rows().end(); ++y) {
                                  for (int x = range.cols().begin(); x != range.cols().end(); ++x) {
                                      image(x, y) =
                                          color_at(scene.world, ray_for_pixel(scene.camera, x, y),
                                                   world_isecs);
                                  }
                              }
                          });

        //        std::vector<intersection> world_isecs;
        //        world_isecs.reserve(world.spheres.size * 2);
        //        for (unsigned y = 0; y != camera.vsize; ++y) {
        //            for (unsigned x = 0; x != camera.hsize; ++x) {
        //                image(x, y) =
        //                    color_at(world, ray_for_pixel(camera, inv_cam_tform, x, y),
        //                    world_isecs);
        //            }
        //        }

        auto stop = std::chrono::steady_clock::now();

        SPDLOG_INFO("Frame #{} drawn in {}", frame,
                    std::chrono::duration<double, std::milli>(stop - start));
    }

    write_ppm(cli.ppm_fname.c_str(), image.as_ppm());
}

} // namespace wt
