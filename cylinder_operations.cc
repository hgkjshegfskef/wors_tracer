#include "cylinder_operations.hh"

#include "cylinder.hh"
#include "intersection.hh"
#include "pnt3.hh"
#include "ray.hh"

#include <spdlog/spdlog.h>

#include <algorithm> // min, max, abs

namespace {

bool check_cap(wt::ray const& r, float t) noexcept {
    float const x = r.origin.x + t * r.direction.x;
    float const z = r.origin.z + t * r.direction.z;
    return (x * x + z * z) < (1.f + 1e-6f);
}

void intersect_caps(wt::cylinder const& cylinder, wt::ray const& object_ray, unsigned shape_id,
                    std::vector<wt::intersection>& world_isecs) noexcept {
    if (!cylinder.closed || std::abs(object_ray.direction.y) < 1e-6f) {
        return;
    }

    float const t1 = (cylinder.min - object_ray.origin.y) / object_ray.direction.y;
    if (check_cap(object_ray, t1)) {
        world_isecs.emplace_back(shape_id, t1);
    }

    float const t2 = (cylinder.max - object_ray.origin.y) / object_ray.direction.y;
    if (check_cap(object_ray, t2)) {
        world_isecs.emplace_back(shape_id, t2);
    }
}

} // namespace

namespace wt {

tform4& tform(cylinder& c) noexcept { return c.tform; }
tform4 const& tform(cylinder const& c) noexcept { return c.tform; }

tform4& inv_tform(cylinder& c) noexcept { return c.inv_tform; }
tform4 const& inv_tform(cylinder const& c) noexcept { return c.inv_tform; }

struct material& mater(cylinder& c) noexcept { return c.material; }
struct material const& mater(cylinder const& c) noexcept { return c.material; }

bool& cast_shadow(cylinder& c) noexcept { return c.cast_shadow; }
bool const& cast_shadow(cylinder const& c) noexcept { return c.cast_shadow; }

void intersect(cylinder const& cylinder, ray const& object_ray, unsigned shape_id,
               std::vector<intersection>& world_isecs) noexcept {
    float const quad_a = object_ray.direction.x * object_ray.direction.x +
                         object_ray.direction.z * object_ray.direction.z;
    if (std::abs(quad_a) < 1e-6f) {
        intersect_caps(cylinder, object_ray, shape_id, world_isecs);
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

    float const t1 = (-quad_b + sqrt_quad_D) / two_quad_a;
    float const t2 = (-quad_b - sqrt_quad_D) / two_quad_a;

    float const y1 = object_ray.origin.y + t1 * object_ray.direction.y;
    if (cylinder.min < y1 && y1 < cylinder.max) {
        world_isecs.emplace_back(shape_id, t1);
    }

    float const y2 = object_ray.origin.y + t2 * object_ray.direction.y;
    if (cylinder.min < y2 && y2 < cylinder.max) {
        world_isecs.emplace_back(shape_id, t2);
    }

    intersect_caps(cylinder, object_ray, shape_id, world_isecs);
}

vec3 normal_at(cylinder const& cylinder, pnt3 const& world_point,
               tform4 const& inv_tform) noexcept {
    pnt3 const object_point = inv_tform * world_point;
    float const dist = object_point.x * object_point.x + object_point.z * object_point.z;
    if (dist < 1.f && object_point.y >= cylinder.max - 1e-6f) {
        return normalize(vec3{0, 1, 0} * inv_tform);
    }
    if (dist < 1.f && object_point.y <= cylinder.min + 1e-6f) {
        return normalize(vec3{0, -1, 0} * inv_tform);
    }
    return normalize(vec3{object_point.x, 0, object_point.z} * inv_tform);
}

} // namespace wt
