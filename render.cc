#include "render.hh"

#include "camera.hh"
#include "canvas.hh"
#include "intersection.hh"
#include "ray.hh"
#include "shape.hh"
#include "world.hh"

// #include <oneapi/tbb/blocked_range2d.h>
// #include <oneapi/tbb/parallel_for.h>
#include <oneapi/tbb.h>

#include <vector>

using namespace oneapi::tbb;

namespace wt {

canvas render(camera const& camera, world const& world) noexcept {
    canvas image{camera.hsize, camera.vsize};
    image.fill({0, 0, 0});

    tform4 const inv_cam_tform = inverse(camera.tform);

    parallel_for(
        blocked_range2d<int>(camera.vsize - 1, -1, 0, camera.hsize), [&](auto const& range) {
            std::vector<intersection> world_isecs;
            world_isecs.reserve(world.shapes.size() * 2);

            for (int y = range.rows().begin(); y != range.rows().end(); --y) {
                for (int x = range.cols().begin(); x != range.cols().end(); ++x) {
                    image(x, y) =
                        color_at(world, ray_for_pixel(camera, inv_cam_tform, x, y), world_isecs);
                }
            }
        });

    //    for (int y = camera.vsize - 1; y >= 0; --y) {
    //        for (unsigned x = 0; x < camera.hsize - 1; ++x) {
    //            // gradient test:
    //            //            float const xoffset = (x + 0.5f) * cam.pixel_size;
    //            //            float const yoffset = (y + 0.5f) * cam.pixel_size;
    //            //            float const world_x = cam.half_width - xoffset;
    //            //            float const world_y = cam.half_height - yoffset;
    //            //            image(x, y) = color{world_x, world_y, 0};
    //
    //            image(x, y) = color_at(world, ray_for_pixel(camera, inv_cam_tform, x, y),
    //            world_isecs);
    //        }
    //    }

    return image;
}

} // namespace wt
