#include "sphere.hh"

#include "intersection.hh"
#include "pnt3.hh"
#include "ray.hh"
#include "vec3.hh"
#include "world.hh"

#include <cmath>  // sqrt
#include <limits> // numeric_limits

namespace wt {

sphere::sphere() noexcept = default;

sphere::sphere(tform4 const& tform) noexcept : sphere(tform, {}) {}

sphere::sphere(tform4 const& tform, struct material const& material) noexcept
    : tform{tform}, inv_tform{inverse(tform)}, material{material} {}

void sphere::intersect(ray const& object_ray, unsigned shape_id,
                       std::vector<intersection>& world_isecs) const noexcept {
    //  Sphere equation: (x-c)⋅(x-c) = r²
    //  Ray equation: x = o + du
    //
    //  x: intersection, a point on the sphere and on the ray
    //  c: center of the sphere (point)
    pnt3 const c{0.f, 0.f, 0.f}; // sphere is at 0
    // r: radius of the sphere
    //  (for our sphere, radius is always 1)
    // o: origin of the ray
    pnt3 const& o = object_ray.origin;
    // d: distance from ray origin to intersection with sphere
    //  (this is what needs to be found)
    // u: ray direction vector (NOT unit len)
    vec3 const& u = object_ray.direction;

    vec3 const oc = o - c;
    float const uoc = dot(u, oc);
    float const quad_a = dot(u, u);
    float const quad_b = 2.f * uoc;
    float const quad_c = dot(oc, oc) - 1.f;

    float const quad_D = quad_b * quad_b - 4.f * quad_a * quad_c;
    if (quad_D < 0) {
        return;
    }

    float const sqrt_quad_D = std::sqrt(quad_D);
    float const two_quad_a = 2.f * quad_a;
    world_isecs.emplace_back(shape_id, (-quad_b + sqrt_quad_D) / two_quad_a);
    world_isecs.emplace_back(shape_id, (-quad_b - sqrt_quad_D) / two_quad_a);
}

} // namespace wt
