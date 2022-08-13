#pragma once

#include "tform4.hh"

namespace wt {

struct camera {
    unsigned hsize; // in px;
    unsigned vsize; // in px;
    float pixel_size;
    float half_width;
    float half_height;
    tform4 inv_tform;

    camera() noexcept = default;
    camera(unsigned hsize, unsigned vsize, float fov) noexcept;
};

struct vec3;
struct pnt3;
struct ray;

// Camera transformation matrix. up is unit len
tform4 view(pnt3 const& from, pnt3 const& to, vec3 const& up) noexcept;

namespace v2 {
tform4 view(pnt3 const& from, pnt3 const& to, vec3 const& up) noexcept;
} // namespace v2

namespace v3 {
tform4 view(pnt3 const& from, vec3 const& forward, vec3 const& up) noexcept;
} // namespace v3

ray ray_for_pixel(camera const& cam, float px, float py) noexcept;

} // namespace wt
