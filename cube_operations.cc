#include "cube_operations.hh"

#include "cube.hh"
#include "intersection.hh"
#include "pnt3.hh"
#include "ray.hh"

#include <algorithm> // min, max, abs

namespace {

struct ts {
    float tmin;
    float tmax;
};

ts check_axis(float origin, float direction) noexcept {
    float const tmin_numerator = -1 - origin;
    float const tmax_numerator = 1 - origin;
    float const tmin = tmin_numerator / direction;
    float const tmax = tmax_numerator / direction;
    if (tmin > tmax) {
        return {tmax, tmin};
    }
    return {tmin, tmax};
}

} // namespace

namespace wt {

tform4& tform(cube& c) noexcept { return c.tform; }
tform4 const& tform(cube const& c) noexcept { return c.tform; }

tform4& inv_tform(cube& c) noexcept { return c.inv_tform; }
tform4 const& inv_tform(cube const& c) noexcept { return c.inv_tform; }

struct material& mater(cube& c) noexcept { return c.material; }
struct material const& mater(cube const& c) noexcept { return c.material; }

bool& cast_shadow(cube& c) noexcept { return c.cast_shadow; }
bool const& cast_shadow(cube const& c) noexcept { return c.cast_shadow; }

void intersect(cube const& /*unused*/, ray const& object_ray, unsigned shape_id,
               std::vector<intersection>& world_isecs) noexcept {
    ts const x = check_axis(object_ray.origin.x, object_ray.direction.x);
    ts const y = check_axis(object_ray.origin.y, object_ray.direction.y);
    ts const z = check_axis(object_ray.origin.z, object_ray.direction.z);
    float const tmin = std::max({x.tmin, y.tmin, z.tmin});
    float const tmax = std::min({x.tmax, y.tmax, z.tmax});
    if (tmin <= tmax) {
        world_isecs.emplace_back(shape_id, tmin);
        world_isecs.emplace_back(shape_id, tmax);
    }
}

vec3 normal_at(cube const& /*unused*/, pnt3 const& world_point, tform4 const& inv_tform) noexcept {
    pnt3 const object_point = inv_tform * world_point;
    float const abs_x = std::abs(object_point.x);
    float const abs_y = std::abs(object_point.y);
    float const abs_z = std::abs(object_point.z);
    float const abs_max = std::max({abs_x, abs_y, abs_z});
    if (abs_max == abs_x) {
        return vec3{object_point.x, 0, 0} * inv_tform;
    }
    if (abs_max == abs_y) {
        return vec3{0, object_point.y, 0} * inv_tform;
    }
    return vec3{0, 0, object_point.z} * inv_tform;
}

} // namespace wt
