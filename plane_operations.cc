#include "plane_operations.hh"

#include "intersection.hh"
#include "plane.hh"
#include "pnt3.hh"
#include "ray.hh"

#include <cmath> // abs

namespace wt {

tform4& tform(plane& p) noexcept { return p.tform; }
tform4 const& tform(plane const& p) noexcept { return p.tform; }

tform4& inv_tform(plane& p) noexcept { return p.inv_tform; }
tform4 const& inv_tform(plane const& p) noexcept { return p.inv_tform; }

struct material& mater(plane& p) noexcept { return p.material; }
struct material const& mater(plane const& p) noexcept { return p.material; }

bool& cast_shadow(plane& p) noexcept { return p.cast_shadow; }
bool const& cast_shadow(plane const& p) noexcept { return p.cast_shadow; }

void intersect(plane const& /*unused*/, ray const& object_ray, unsigned shape_id,
               std::vector<intersection>& world_isecs) noexcept {
    if (std::abs(object_ray.direction.y) < 1e-6f) {
        return;
    }
    world_isecs.emplace_back(shape_id, -object_ray.origin.y / object_ray.direction.y);
}

vec3 normal_at(plane const& /*unused*/, pnt3 const& /*unused*/, tform4 const& inv_tform) noexcept {
    return vec3{0, 1, 0} * inv_tform;
}

} // namespace wt
