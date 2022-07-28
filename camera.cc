#include "camera.hh"

#include "pnt3.hh"
#include "ray.hh"
#include "tform4.hh"
#include "vec3.hh"

#include <cassert>
#include <cmath> // tan

namespace wt {

camera::camera(unsigned hsize, unsigned vsize, float fov) noexcept
    : hsize{hsize}, vsize{vsize}, fov{fov}, tform{} {
    float half_view = std::tan(fov / 2.f);
    float aspect = float(hsize) / float(vsize);
    if (aspect >= 1.f) {
        half_width = half_view;
        half_height = half_view / aspect;
    } else {
        half_width = half_view * aspect;
        half_height = half_view;
    }
    pixel_size = (half_width * 2.f) / hsize;
}

tform4 view(pnt3 const& from, pnt3 const& to, vec3 const& up) noexcept {
    vec3 const forward = normalize(to - from);
    vec3 const left = cross(forward, normalize(up));
    vec3 const true_up = cross(left, forward);
    tform4 orientation{left.x,     left.y,     left.z,     0, //
                       true_up.x,  true_up.y,  true_up.z,  0, //
                       -forward.x, -forward.y, -forward.z, 0};
    return orientation * tform4::translate({-from.x, -from.y, -from.z});
}

namespace v2 {

tform4 view(pnt3 const& from, pnt3 const& to, vec3 const& up) noexcept {
    vec3 const forward = normalize(to - from);
    vec3 const left = normalize(cross(forward, normalize(up)));
    vec3 const true_up = -normalize(cross(left, forward));    // !!!
    tform4 orientation{left.x,     left.y,     left.z,     0, //
                       true_up.x,  true_up.y,  true_up.z,  0, //
                       -forward.x, -forward.y, -forward.z, 0};
    return orientation * tform4::translate({-from.x, -from.y, -from.z});
}

} // namespace v2

ray ray_for_pixel(camera const& cam, float px, float py) noexcept {
    // offset from edge of canvas to pixel center
    float const xoffset = (px + 0.5f) * cam.pixel_size;
    float const yoffset = (py + 0.5f) * cam.pixel_size;

    // cam looks towards -z, x is to the left
    float const world_x = cam.half_width - xoffset;
    float const world_y = cam.half_height - yoffset;
    assert(-1.f <= world_x && world_x <= 1.f);
    assert(-1.f <= world_y && world_y <= 1.f);

    tform4 const inv_cam_tform = inverse(cam.tform);
    // canvas is at z=-1
    pnt3 const pixel = inv_cam_tform * pnt3{world_x, world_y, -1};
    pnt3 const origin = inv_cam_tform * pnt3{0, 0, 0};
    vec3 const direction = normalize(pixel - origin);

    return ray{origin, direction};
}

} // namespace wt
