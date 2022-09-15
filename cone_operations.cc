#include "cone_operations.hh"

#include "cone.hh"
#include "intersection.hh"
#include "pnt3.hh"
#include "ray.hh"

#include <spdlog/spdlog.h>

#include <algorithm> // min, max, abs

namespace {

bool check_cap(wt::ray const& r, float t, float radius) noexcept {
    float const x = r.origin.x + t * r.direction.x;
    float const z = r.origin.z + t * r.direction.z;
    return (x * x + z * z) < (radius + 1e-6f);
}

void intersect_caps(wt::cone const& cone, wt::ray const& object_ray, unsigned shape_id,
                    std::vector<wt::intersection>& world_isecs) noexcept {
    if (!cone.closed || std::abs(object_ray.direction.y) < 1e-6f) {
        return;
    }

    float const t1 = (cone.min - object_ray.origin.y) / object_ray.direction.y;
    if (check_cap(object_ray, t1, std::abs(cone.min))) {
        world_isecs.emplace_back(shape_id, t1);
    }

    float const t2 = (cone.max - object_ray.origin.y) / object_ray.direction.y;
    if (check_cap(object_ray, t2, std::abs(cone.max))) {
        world_isecs.emplace_back(shape_id, t2);
    }
}

} // namespace

namespace wt {

void intersect(cone const& cone, ray const& object_ray, unsigned shape_id,
               std::vector<intersection>& world_isecs) noexcept {
    float const quad_a = object_ray.direction.x * object_ray.direction.x -
                         object_ray.direction.y * object_ray.direction.y +
                         object_ray.direction.z * object_ray.direction.z;
    float const quad_b = 2.f * object_ray.origin.x * object_ray.direction.x -
                         2.f * object_ray.origin.y * object_ray.direction.y +
                         2.f * object_ray.origin.z * object_ray.direction.z;
    float const quad_c = object_ray.origin.x * object_ray.origin.x -
                         object_ray.origin.y * object_ray.origin.y +
                         object_ray.origin.z * object_ray.origin.z;
    if (std::abs(quad_a) < 1e-6f) {
        if (std::abs(quad_b) < 1e-6f) {
            return;
        }
        world_isecs.emplace_back(shape_id, -quad_c / (2.f * quad_b));
        intersect_caps(cone, object_ray, shape_id, world_isecs);
        return;
    }

    float quad_D = quad_b * quad_b - 4.f * quad_a * quad_c;
    if (quad_D < -1e-6f) {
        return;
    }
    quad_D = std::abs(quad_D);

    float const sqrt_quad_D = std::sqrt(quad_D);
    float const two_quad_a = 2.f * quad_a;

    float const t1 = (-quad_b + sqrt_quad_D) / two_quad_a;
    float const t2 = (-quad_b - sqrt_quad_D) / two_quad_a;

    float const y1 = object_ray.origin.y + t1 * object_ray.direction.y;
    if (cone.min < y1 && y1 < cone.max) {
        world_isecs.emplace_back(shape_id, t1);
    }

    float const y2 = object_ray.origin.y + t2 * object_ray.direction.y;
    if (cone.min < y2 && y2 < cone.max) {
        world_isecs.emplace_back(shape_id, t2);
    }

    intersect_caps(cone, object_ray, shape_id, world_isecs);
}

vec3 normal_at(cone const& cone, pnt3 const& world_point, tform4 const& inv_tform) noexcept {
    pnt3 const object_point = inv_tform * world_point;
    float const dist = object_point.x * object_point.x + object_point.z * object_point.z;
    if (dist < 1.f && object_point.y >= (cone.max - 1e-6f)) {
        return normalize(vec3{0, 1, 0} * inv_tform);
    }
    if (dist < 1.f && object_point.y <= (cone.min + 1e-6f)) {
        return normalize(vec3{0, -1, 0} * inv_tform);
    }

    float y = std::sqrt(object_point.x * object_point.x + object_point.z * object_point.z);
    if (object_point.y > (0 - 1e-6f)) {
        y = -y;
    }
    return normalize(vec3{object_point.x, y, object_point.z} * inv_tform); // can be nan
}

} // namespace wt
