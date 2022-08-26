#include "stub_shape_operations.hh"

#include "intersection.hh"
#include "pnt3.hh"
#include "ray.hh"
#include "stub_shape.hh"

#include <spdlog/spdlog.h>

#include <cstdlib> // exit

namespace wt {

tform4& tform(stub_shape& s) noexcept { return s.tform; }
tform4 const& tform(stub_shape const& s) noexcept { return s.tform; }

tform4& inv_tform(stub_shape& s) noexcept { return s.inv_tform; }
tform4 const& inv_tform(stub_shape const& s) noexcept { return s.inv_tform; }

struct material& mater(stub_shape& s) noexcept { return s.material; }
struct material const& mater(stub_shape const& s) noexcept { return s.material; }

void intersect(stub_shape const& /*unused*/, ray const& /*unused*/, unsigned /*unused*/,
               std::vector<intersection>& /*unused*/) noexcept {
    SPDLOG_ERROR("{} is a stub and shouldn't be called", __PRETTY_FUNCTION__);
    std::exit(1);
}

vec3 normal_at(stub_shape const& /*unused*/, pnt3 const& /*unused*/,
               tform4 const& /*unused*/) noexcept {
    SPDLOG_ERROR("{} is a stub and shouldn't be called", __PRETTY_FUNCTION__);
    std::exit(1);
}

} // namespace wt
