#include "sphere.hh"

namespace wt {

sphere::sphere() noexcept : tform{std::make_shared<tform4>(tform4::identity())} {}

sphere::sphere(std::shared_ptr<tform4> tform) noexcept : tform{std::move(tform)} {}

} // namespace wt
