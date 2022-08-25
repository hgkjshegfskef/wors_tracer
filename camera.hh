#pragma once

#include "pnt3.hh"
#include "tform4.hh"

namespace wt {

struct look_at;

struct camera {
    unsigned hsize; // in px;
    unsigned vsize; // in px;
    float pixel_size;
    float half_width;
    float half_height;
    tform4 inv_tform;

    camera() noexcept;
    camera(unsigned hsize, unsigned vsize, float fov) noexcept;
    camera(unsigned hsize, unsigned vsize, float fov, look_at const& look_at) noexcept;
};

struct vec3;
struct pnt3;
struct ray;

// Camera transformation matrix. up is unit len
tform4 view(pnt3 const& from, pnt3 const& to, vec3 const& up) noexcept;
tform4 view(pnt3 const& from, vec3 const& forward, vec3 const& up) noexcept;

ray ray_for_pixel(camera const& cam, float px, float py) noexcept;

} // namespace wt
