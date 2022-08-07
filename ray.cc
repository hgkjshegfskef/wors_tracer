#include "ray.hh"

#include "intersection.hh"
#include "sphere.hh"

#include <cmath>  // abs, sqrt, pow
#include <limits> // numeric_limits<float>

// namespace {
//
//// Given 2 floats, return the smaller non-negative one
// std::optional<float> min_nonnegative(float t1, float t2) noexcept {
//     if (t1 < 0) {
//         // t2 is the only candidate
//         if (t2 < 0) {
//             return std::nullopt;
//         }
//         return t2;
//     } else if (t2 < 0) {
//         // t1 is the only candidate
//         if (t1 < 0) {
//             return std::nullopt;
//         }
//         return t1;
//     } else {
//         // both are candidates
//         if (t1 < t2) {
//             return t1;
//         }
//         return t2;
//     }
// }
//
// } // namespace

namespace wt {

ray::ray(const pnt3& p, const vec3& v) noexcept : origin{p}, direction{v} {}

pnt3 position(pnt3 const& origin, vec3 const& direction, float t) noexcept {
    return origin + t * direction;
}

// std::optional<float> intersect(ray const& r, sphere const& s) noexcept {
//     // Account for the transformations applied to sphere (so, apply the inverse of them to ray).
//     tform4 const inv = inverse(s.tform);
//     ray const transformed{inv * r.origin, normalize(inv * r.direction)};
//
//     // Sphere equation: (x-c)⋅(x-c) = r²
//     // Ray equation: x = o + du
//     //
//     // x: intersection, a point on the sphere and on the ray
//     // c: center of the sphere (point)
//     pnt3 const c{0.f, 0.f, 0.f}; // sphere is at 0
//     // r: radius of the sphere
//     //  (for our sphere, radius is always 1)
//     // o: origin of the ray
//     pnt3 const& o = transformed.origin;
//     // d: distance from ray origin to intersection with sphere
//     //  (this is what needs to be found)
//     // u: ray direction vector (unit len)
//     vec3 const& u = transformed.direction;
//
//     // Solving the above for d:
//     //
//     // t₁₋₂ = -u⋅(o-c) ± √D
//     // D = [u⋅(o-c)]² - ((o-c)⋅(o-c) - 1)
//
//     vec3 const oc = o - c;
//     float const uoc = dot(u, oc);
//     float const D = std::pow(uoc, 2) - (dot(oc, oc) - 1);
//     if (D < 0) {
//         return {};
//     }
//     float const sqrt_D = std::sqrt(D);
//     float const t1 = -uoc + sqrt_D;
//     float const t2 = -uoc - sqrt_D;
//
//     return min_nonnegative(t1, t2);
// }

// std::optional<float> intersect_sphere(ray const& r) noexcept {
//     // Sphere equation: (x-c)⋅(x-c) = r²
//     // Ray equation: x = o + du
//     //
//     // x: intersection, a point on the sphere and on the ray
//     // c: center of the sphere (point)
//     pnt3 const c{0.f, 0.f, 0.f}; // sphere is at 0
//     // r: radius of the sphere
//     //  (for our sphere, radius is always 1)
//     // o: origin of the ray
//     pnt3 const& o = r.origin;
//     // d: distance from ray origin to intersection with sphere
//     //  (this is what needs to be found)
//     // u: ray direction vector (unit len)
//     vec3 const& u = r.direction;
//
//     // Solving the above for d:
//     //
//     // t₁₋₂ = -u⋅(o-c) ± √D
//     // D = [u⋅(o-c)]² - ((o-c)⋅(o-c) - 1)
//
//     vec3 const oc = o - c;
//     float const uoc = dot(u, oc);
//     float const D = std::pow(uoc, 2) - (dot(oc, oc) - 1);
//     if (D < 0) {
//         return {};
//     }
//     float const sqrt_D = std::sqrt(D);
//     float const t1 = -uoc + sqrt_D;
//     float const t2 = -uoc - sqrt_D;
//
//     return min_nonnegative(t1, t2);
// }

// namespace v2 {
//
// std::array<intersection, 2> intersect(ray const& world_r, sphere const& s) noexcept {
//     tform4 const inv_tform = inverse(s.tform);
//     ray const& object_r{
//         inv_tform * world_r.origin,
//         // do not normalize the result, so we get the t straight for the world space
//         // without the need to convert it first from the object space (why convert? because the t
//         we
//         // get is for the transformed ray, not for the world space ray). this results in u (see
//         // below) not being a unit vector, which results in more calculation. but it is worth it
//         inv_tform * world_r.direction};
//
//     // Sphere equation: (x-c)⋅(x-c) = r²
//     // Ray equation: x = o + du
//     //
//     // x: intersection, a point on the sphere and on the ray
//     // c: center of the sphere (point)
//     pnt3 const c{0.f, 0.f, 0.f}; // sphere is at 0
//     // r: radius of the sphere
//     //  (for our sphere, radius is always 1)
//     // o: origin of the ray
//     pnt3 const& o = object_r.origin;
//     // d: distance from ray origin to intersection with sphere
//     //  (this is what needs to be found)
//     // u: ray direction vector (NOT unit len)
//     vec3 const& u = object_r.direction;
//
//     vec3 const oc = o - c;
//     float const uoc = dot(u, oc);
//     float const quad_a = dot(u, u);
//     float const quad_b = 2.f * uoc;
//     float const quad_c = dot(oc, oc) - 1.f;
//
//     float const quad_D = std::pow(quad_b, 2.f) - 4.f * quad_a * quad_c;
//     if (quad_D < 0) {
//         return {};
//     }
//     float const sqrt_quad_D = std::sqrt(quad_D);
//     float const two_quad_a = 2.f * quad_a;
//     return std::array{intersection{&s, (-quad_b + sqrt_quad_D) / two_quad_a},
//                       intersection{&s, (-quad_b - sqrt_quad_D) / two_quad_a}};
// }

//} // namespace v2

//long hit(std::vector<intersection> const& isecs) noexcept {
//    float min_t{std::numeric_limits<float>::max()};
//    for (size_t i = 0; i < isecs.size(); ++i) {
//        if (isecs[i].t > 0 )
//    }
//
//    for (auto const& isec : isecs) {
//        if (!isec.empty() && isec.t > 0 && isec.t < min_t.t) {
//            min_t = isec;
//        }
//    }
//    return min_t;
//}

// template intersection hit<std::vector<intersection>>(std::vector<intersection> const&) noexcept;
// template intersection hit<std::array<intersection, 2>>(std::array<intersection, 2> const&)
// noexcept;

float dist(pnt3 const& q, pnt3 const& p, vec3 const& v, bool v_is_normal) noexcept {
    vec3 const a = cross(q - p, v);
    if (v_is_normal) {
        return std::sqrt(dot(a, a));
    }
    return std::sqrt(dot(a, a) / dot(v, v));
}

} // namespace wt
