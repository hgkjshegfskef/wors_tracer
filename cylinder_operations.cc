#include "cylinder_operations.hh"

#include "cylinder.hh"
#include "intersection.hh"
#include "pnt3.hh"
#include "ray.hh"

#include <spdlog/spdlog.h>

#include <algorithm> // min, max, abs

namespace wt {

tform4& tform(cylinder& c) noexcept { return c.tform; }
tform4 const& tform(cylinder const& c) noexcept { return c.tform; }

tform4& inv_tform(cylinder& c) noexcept { return c.inv_tform; }
tform4 const& inv_tform(cylinder const& c) noexcept { return c.inv_tform; }

struct material& mater(cylinder& c) noexcept { return c.material; }
struct material const& mater(cylinder const& c) noexcept { return c.material; }

bool& cast_shadow(cylinder& c) noexcept { return c.cast_shadow; }
bool const& cast_shadow(cylinder const& c) noexcept { return c.cast_shadow; }

void intersect(cylinder const& /*unused*/, ray const& object_ray, unsigned shape_id,
               std::vector<intersection>& world_isecs) noexcept {
    float const quad_a = object_ray.direction.x * object_ray.direction.x +
                         object_ray.direction.z * object_ray.direction.z;
    if (std::abs(quad_a) < 1e-6f) {
        return;
    }
    float const quad_b = 2.f * object_ray.origin.x * object_ray.direction.x +
                         2.f * object_ray.origin.z * object_ray.direction.z;
    float const quad_c =
        object_ray.origin.x * object_ray.origin.x + object_ray.origin.z * object_ray.origin.z - 1.f;

    float const quad_D = quad_b * quad_b - 4.f * quad_a * quad_c;
    if (quad_D < 0) {
        return;
    }

    float const sqrt_quad_D = std::sqrt(quad_D);
    float const two_quad_a = 2.f * quad_a;
    world_isecs.emplace_back(shape_id, (-quad_b + sqrt_quad_D) / two_quad_a);
    world_isecs.emplace_back(shape_id, (-quad_b - sqrt_quad_D) / two_quad_a);
}

vec3 normal_at(cylinder const& /*unused*/, pnt3 const& world_point,
               tform4 const& inv_tform) noexcept {
    pnt3 const object_point = inv_tform * world_point;
    return normalize(vec3{object_point.x, 0, object_point.z} * inv_tform);
}

} // namespace wt
