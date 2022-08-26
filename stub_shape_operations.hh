#pragma once

#include <vector>

namespace wt {

struct stub_shape;
class tform4;
struct material;
struct ray;
struct intersection;
struct vec3;
struct pnt3;

tform4& tform(stub_shape& s) noexcept;
tform4 const& tform(stub_shape const& s) noexcept;

tform4& inv_tform(stub_shape& s) noexcept;
tform4 const& inv_tform(stub_shape const& s) noexcept;

struct material& mater(stub_shape& s) noexcept;
struct material const& mater(stub_shape const& s) noexcept;

void intersect(stub_shape const& /*unused*/, ray const& /*unused*/, unsigned /*unused*/,
               std::vector<intersection>& /*unused*/) noexcept;

vec3 normal_at(stub_shape const& /*unused*/, pnt3 const& /*unused*/,
               tform4 const& /*unused*/) noexcept;

} // namespace wt
