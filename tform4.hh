// Some code is based on "Foundations of Game Engine Development, Volume 1: Mathematics", E.Lengyel

#pragma once

#include "mat3.hh"
#include "mat4.hh"
#include "pnt3.hh"
#include "vec3.hh"

#include <fmt/core.h>

namespace wt {

class tform4 final : public mat4 {
  public:
    tform4() noexcept;

    tform4(float n00, float n01, float n02, float n03, //
           float n10, float n11, float n12, float n13, //
           float n20, float n21, float n22, float n23) noexcept;

    tform4(vec3 const& a, vec3 const& b, vec3 const& c, pnt3 const& p) noexcept;

    tform4(mat3 const& M) noexcept;

    pnt3 const& get_translation() const noexcept;
    tform4 const& set_translation(pnt3 const& p) noexcept;

    vec3& operator[](int j) noexcept;
    vec3 const& operator[](int j) const noexcept;
};

tform4 inverse(tform4 const& H) noexcept;

tform4 operator*(tform4 const& A, tform4 const& B) noexcept;

// transform column vector
vec3 operator*(tform4 const& H, vec3 const& v) noexcept;

// transform position vector
pnt3 operator*(tform4 const& H, pnt3 const& p) noexcept;

// transform normal vector (as row) from coordinate system B to coordinate system A: nM^-1, where M
// is the upper left 3x3 of H.
// Therefore, H is assumed to be inverted.
// If upper-left 3x3 of H is orthogonal, normal vector can be transformed just like regular vector:
// Mn.
vec3 operator*(vec3 const& n, tform4 const& H) noexcept;

} // namespace wt

template <> struct fmt::formatter<wt::tform4> : fmt::formatter<float> {
    auto format(wt::tform4 const& mat, fmt::format_context& ctx) -> decltype(ctx.out());
};

extern template struct fmt::formatter<wt::tform4>;
