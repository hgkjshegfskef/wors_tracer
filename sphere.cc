#include "sphere.hh"

#include <utility> // move

namespace wt {

sphere::sphere() noexcept : tform{} {}

sphere::sphere(tform4 tform) noexcept : tform{std::move(tform)} {}

} // namespace wt
