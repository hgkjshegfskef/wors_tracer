#include "shading.hh"

#include "ray.hh"
#include "sphere.hh"

namespace wt {

shading::shading(intersection const& isec, ray const& r) noexcept
    : isec{isec}, isec_pnt{position(r.origin, r.direction, isec.t)}, eye{-r.direction},
      normal{normal_at(*isec.s, isec_pnt)}, inside{false} {
    if (dot(normal, eye) < 0) {
        inside = true;
        normal = -normal;
    }
}

} // namespace wt
