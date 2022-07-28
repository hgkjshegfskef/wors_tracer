#include "shading.hh"

#include "ray.hh"
#include "shape.hh"
#include "sphere_operations.hh"

namespace wt {

shading::shading(intersection const& isec, ray const& r) noexcept
    : isec{isec}, isec_pnt{position(r.origin, r.direction, isec.t)}, over_pnt{}, eye{-r.direction},
      normal{normal_at(*isec.s, isec_pnt)}, inside{false} {
    if (dot(normal, eye) < 0) {
        inside = true;
        normal = -normal;
    }
    over_pnt = isec_pnt + normal * 1e-2f; // remove acne
}

} // namespace wt
