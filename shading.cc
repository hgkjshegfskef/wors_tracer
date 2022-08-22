#include "shading.hh"

#include "ray.hh"
#include "shape.hh"
#include "tform4.hh"

namespace wt {

shading::shading(intersection const& isec, ray const& r, shape const& s) noexcept
    : isec{isec}, isec_pnt{position(r.origin, r.direction, isec.t)}, over_pnt{}, eye{-r.direction},
      normal{normal_at(s, isec_pnt, inv_tform(s))}, inside{false} {
    if (dot(normal, eye) < 0) {
        inside = true;
        normal = -normal;
    }
    // TODO: per-scene acne epsilon
    over_pnt = isec_pnt + normal * 1e-3f; // remove acne
}

} // namespace wt
