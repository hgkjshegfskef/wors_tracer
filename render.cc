#include "render.hh"

#include "camera.hh"
#include "canvas.hh"
#include "ray.hh"
#include "world.hh"

namespace wt {

canvas render(camera const& cam, world const& w) noexcept {
    canvas image{cam.hsize, cam.vsize};
    image.fill({0, 0, 0});
    for (int y = cam.vsize - 1; y >= 0; --y) {
        for (unsigned x = 0; x < cam.hsize - 1; ++x) {
            // gradient test:
            //            float const xoffset = (x + 0.5f) * cam.pixel_size;
            //            float const yoffset = (y + 0.5f) * cam.pixel_size;
            //            float const world_x = cam.half_width - xoffset;
            //            float const world_y = cam.half_height - yoffset;
            //            image(x, y) = color{world_x, world_y, 0};

            image(x, y) = color_at(w, ray_for_pixel(cam, x, y));
        }
    }
    return image;
}

} // namespace wt
