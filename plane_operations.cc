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

void intersect(plane const& /*unused*/, ray const& object_ray, unsigned shape_id,
               std::vector<intersection>& world_isecs) noexcept {
    if (std::abs(object_ray.direction.y) < 1e-6f) {
        return;
    }
    world_isecs.emplace_back(shape_id, -object_ray.origin.y / object_ray.direction.y);
}

} // namespace wt
