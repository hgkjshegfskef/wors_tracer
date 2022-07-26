#include "sphere.hh"

#include "pnt3.hh"
#include "vec3.hh"

#include <utility> // move

namespace wt {

sphere::sphere() noexcept : tform{}, mat{} {}

sphere::sphere(tform4 tform) noexcept : tform{std::move(tform)}, mat{} {}

sphere::sphere(tform4 tform, struct material material) noexcept
    : tform{std::move(tform)}, mat{std::move(material)} {}

vec3 normal_at(sphere const& s, pnt3 const& world_point) noexcept {
    tform4 const inv_tform = inverse(s.tform);
    pnt3 const object_point = inv_tform * world_point;
    vec3 const object_vector = normalize(object_point - pnt3{0, 0, 0});
    return normalize(object_vector * inv_tform);
}

} // namespace wt
