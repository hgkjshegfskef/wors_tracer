#include "ray.hh"

#include "sphere.hh"

#include <spdlog/spdlog.h>

#include <cassert>
#include <cmath> // abs, sqrt, pow

namespace {

// Given 2 floats, return the smaller non-negative one
std::optional<float> min_nonnegative(float t1, float t2) noexcept {
    if (t1 < 0) {
        // t2 is the only candidate
        if (t2 < 0) {
            return std::nullopt;
        }
        return t2;
    } else if (t2 < 0) {
        // t1 is the only candidate
        if (t1 < 0) {
            return std::nullopt;
        }
        return t1;
    } else {
        // both are candidates
        if (t1 < t2) {
            return t1;
        }
        return t2;
    }
}

} // namespace

namespace wt {

ray::ray(const pnt3& p, const vec3& v) noexcept : origin{p}, direction{v} {}

pnt3 position(pnt3 const& origin, vec3 const& direction, float t) noexcept {
    return origin + t * direction;
}

std::optional<float> intersect(ray const& r, sphere const& s) noexcept {
    // Account for the transformations applied to sphere (so, apply the inverse of them to ray).
    tform4 const inv = inverse(*s.tform);
    ray const transformed{inv * r.origin, normalize(inv * r.direction)};
//    SPDLOG_TRACE("Transformed ray: origin: {}, direction: {}", transformed.origin,
//                 transformed.direction);

    // Sphere equation: (x-c)⋅(x-c) = r²
    // Ray equation: x = o + du
    //
    // x: intersection, a point on the sphere and on the ray
    // c: center of the sphere (point)
    pnt3 const c{0.f, 0.f, 0.f}; // sphere is at 0
    // r: radius of the sphere
    //  (for our sphere, radius is always 1)
    // o: origin of the ray
    pnt3 const& o = transformed.origin;
    // d: distance from ray origin to intersection with sphere
    //  (this is what needs to be found)
    // u: ray direction vector (unit len)
    vec3 const& u = transformed.direction;

    // Solving the above for d:
    //
    // t₁₋₂ = -u⋅(o-c) ± √D
    // D = [u⋅(o-c)]² - ((o-c)⋅(o-c) - 1)

    vec3 const oc = o - c;
    float const uoc = dot(u, oc);
    float const D = std::pow(uoc, 2) - (dot(oc, oc) - 1);
    if (D < 0) {
        return {};
    }
    float const sqrt_D = std::sqrt(D);
//    SPDLOG_TRACE("oc: {}, uoc: {}, D: {}, sqrt_D: {}", oc, uoc, D, sqrt_D);

    float const t1 = -uoc + sqrt_D;
    float const t2 = -uoc - sqrt_D;

//    SPDLOG_TRACE("In object space, t1 and t2 are {}, {}", t1, t2);
    //
    //    pnt3 const p1 = position(o, u, t1);
    //    pnt3 const p2 = position(o, u, t2);
    //    SPDLOG_TRACE("In object space, points are {}, {}", p1, p2);
    //
    //    pnt3 const world_p1 = *s.tform * p1;
    //    pnt3 const world_p2 = *s.tform * p2;
    //    SPDLOG_TRACE("In world space, points are {}, {}", world_p1, world_p2);

    return min_nonnegative(t1, t2);

    // Create vector from the sphere center towards ray origin (sphere is at 0).
    //    vec3 sphere_to_ray = normalize(pnt3{0.f, 0.f, 0.f} - transformed.origin);

    //    double const a = dot(transformed.direction, transformed.direction);
    //    double const b = 2. * dot(transformed.direction, sphere_to_ray);
    //    double const c = dot(sphere_to_ray, sphere_to_ray) - 1.;
    //    double const disc = b * b - 4. * a * c;
    //
    //    double const two_a = 2. * a;
    //    //    if (disc == 0) { // == 0
    //    if (std::abs(disc) < 1e-6) { // == 0
    //        SPDLOG_TRACE("disc == 0 (real: {})", disc);
    //        float const result = -b / two_a;
    //        return {{result, s}, {result, s}};
    //    }
    //
    //    // Only now check for no solutions, since near zero result (above) can be negative as
    //    well. if (disc < 0.f) {
    //        //        SPDLOG_TRACE("no solutions, disc < 0");
    //        return {};
    //    }
    //
    //    double const sqrt_disc = std::sqrt(disc);
    //    return {{float((-b - sqrt_disc) / two_a), s}, {float((-b + sqrt_disc) / two_a), s}};
}

// intersection const* hit(std::vector<intersection> const& intersections) noexcept {
//     intersection const* first_intersection = nullptr;
//     if (!intersections.empty()) {
//         float smallest_nonnegative{std::numeric_limits<float>::max()};
//         for (auto const& i : intersections) {
//             SPDLOG_TRACE("intersection: {}, position: ", i.t);
//             if (i.t >= 0 && i.t < smallest_nonnegative) {
//                 smallest_nonnegative = i.t;
//                 first_intersection = &i;
//             }
//         }
//     }
//     return first_intersection;
// }

} // namespace wt
