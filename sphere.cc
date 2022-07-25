#include "sphere.hh"

#include <utility> // move

namespace wt {

sphere::sphere() noexcept : tform{}, mat{} {}

sphere::sphere(tform4 tform) noexcept : tform{std::move(tform)}, mat{} {}

sphere::sphere(tform4 tform, struct material material) noexcept
    : tform{std::move(tform)}, mat{std::move(material)} {}

} // namespace wt
