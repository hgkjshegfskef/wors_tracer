#include "sphere.hh"

#include "intersection.hh"
#include "pnt3.hh"
#include "ray.hh"
#include "vec3.hh"

#include <cmath> // pow, sqrt

namespace wt {

sphere::sphere() noexcept = default;

sphere::sphere(tform4 const& tform) noexcept : tform{tform}, material{} {}

sphere::sphere(tform4 const& tform, struct material const& material) noexcept
    : tform{tform}, material{material} {}

tform4& get_tform(sphere& sphere) noexcept { return sphere.tform; }
tform4 const& get_tform(sphere const& sphere) noexcept { return sphere.tform; }

material& get_material(sphere& sphere) noexcept { return sphere.material; }
material const& get_material(sphere const& sphere) noexcept { return sphere.material; }

vec3 normal_at(sphere const& sphere, pnt3 const& world_point) noexcept {
    tform4 const inv_tform = inverse(sphere.tform);
    pnt3 const object_point = inv_tform * world_point;
    vec3 const object_vector = normalize(object_point - pnt3{0, 0, 0});
    return normalize(object_vector * inv_tform);
}

std::array<intersection, 2> intersect(sphere const& s, ray const& world_ray) noexcept {
    tform4 const inv_tform = inverse(s.tform);
    ray const object_ray{
        inv_tform * world_ray.origin,
        // do not normalize the result, so we get the t straight for the world space without the
        // need to convert it first from the object space (why convert? because the t we get is for
        // the transformed ray, not for the world space ray). this results in u (see below) not
        // being a unit vector, which results in more calculation. but it is worth it
        inv_tform * world_ray.direction};

    // Sphere equation: (x-c)⋅(x-c) = r²
    // Ray equation: x = o + du
    //
    // x: intersection, a point on the sphere and on the ray
    // c: center of the sphere (point)
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
        return {};
    }
    float const sqrt_quad_D = std::sqrt(quad_D);
    float const two_quad_a = 2.f * quad_a;
    // TODO: pointer to what?
    return std::array{intersection{&s, (-quad_b + sqrt_quad_D) / two_quad_a},
                      intersection{&s, (-quad_b - sqrt_quad_D) / two_quad_a}};
}

} // namespace wt
