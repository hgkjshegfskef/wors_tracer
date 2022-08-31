#include "shading.hh"

#include "mat3.hh"
#include "material.hh"
#include "ray.hh"
#include "shape.hh"
#include "tform4.hh"

#include <algorithm>
#include <deque>
#include <list>

namespace wt {

shading::shading(std::vector<shape> const& shapes, ray const& r,
                 std::vector<intersection> const& world_isecs, intersection hit) noexcept
    : isec{std::move(hit)}, isec_pnt{position(r.origin, r.direction, isec.t)}, over_pnt{},
      under_pnt{}, eye{-r.direction}, normal{normal_at(shapes[isec.shape_id], isec_pnt,
                                                       inv_tform(shapes[isec.shape_id]))},
      reflect{}, n1{}, n2{}, inside{false} {
    if (dot(normal, eye) < 0) {
        inside = true;
        normal = -normal;
    }
    reflect = tform4{reflection(normal)} * r.direction;
    // TODO: per-scene acne epsilon
    over_pnt = isec_pnt + normal * 1e-5f;
    under_pnt = isec_pnt - normal * 1e-5f;

    // TODO: dynamic allocations here
    std::list<unsigned> shape_ids;
    for (auto const& i : world_isecs) {
        if (i == isec) {
            if (shape_ids.empty()) {
                n1 = 1.f;
            } else {
                n1 = mater(shapes[shape_ids.back()]).refraction_index;
            }
        }
        if (auto it = std::find(shape_ids.begin(), shape_ids.end(), i.shape_id);
            it != shape_ids.end()) {
            shape_ids.erase(it);
        } else {
            shape_ids.push_back(i.shape_id);
        }
        if (i == isec) {
            if (shape_ids.empty()) {
                n2 = 1.f;
            } else {
                n2 = mater(shapes[shape_ids.back()]).refraction_index;
            }
            break;
        }
    }
}

} // namespace wt
