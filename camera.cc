#include "camera.hh"

#include "look_at.hh"
#include "pnt3.hh"
#include "ray.hh"
#include "tform4.hh"
#include "vec3.hh"

#include <cassert>
#include <cmath> // tan

namespace wt {

camera::camera() noexcept = default;

camera::camera(unsigned hsize, unsigned vsize, float fov) noexcept
    : hsize{hsize}, vsize{vsize}, inv_tform{} {
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

camera::camera(unsigned hsize, unsigned vsize, float fov, look_at const& look_at,
               bool invert_y) noexcept
    : camera{hsize, vsize, fov} {
    if (!invert_y) {
        inv_tform = view(look_at.from, look_at.to, look_at.up);
    } else {
        inv_tform = v2::view(look_at.from, look_at.to, look_at.up);
    }
}

tform4 view(pnt3 const& from, pnt3 const& to, vec3 const& up) noexcept {
    vec3 const forward = normalize(to - from);
    vec3 const left = normalize(cross(forward, normalize(up)));
    vec3 const true_up = normalize(cross(left, forward));
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

namespace v3 {
tform4 view(pnt3 const& from, vec3 const& forward, vec3 const& up) noexcept {
    vec3 const left = normalize(cross(normalize(forward), normalize(up)));
    vec3 const true_up = normalize(cross(left, forward));
    tform4 orientation{left.x,     left.y,     left.z,     0, //
                       true_up.x,  true_up.y,  true_up.z,  0, //
                       -forward.x, -forward.y, -forward.z, 0};
    return orientation * tform4::translate({-from.x, -from.y, -from.z});
}
} // namespace v3

ray ray_for_pixel(camera const& cam, float px, float py) noexcept {
    // cam looks towards -z, x is to the left
    float const world_x = cam.half_width - (px + 0.5f) * cam.pixel_size;
    float const world_y = cam.half_height - (py + 0.5f) * cam.pixel_size;
    assert(-1.f <= world_x && world_x <= 1.f);
    assert(-1.f <= world_y && world_y <= 1.f);

    // canvas is at z=-1
    pnt3 const pixel = cam.inv_tform * pnt3{world_x, world_y, -1};
    pnt3 const origin = cam.inv_tform.get_translation();
    vec3 const direction = normalize(pixel - origin);

    return ray{std::move(origin), std::move(direction)};
}

} // namespace wt
