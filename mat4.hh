// Some code is based on "Foundations of Game Engine Development, Volume 1: Mathematics", E.Lengyel

#pragma once

#include "vec4.hh"

namespace wt {

class mat4 {
  public:
    mat4() noexcept = default;

    mat4(float n00, float n01, float n02, float n03, //
         float n10, float n11, float n12, float n13, //
         float n20, float n21, float n22, float n23, //
         float n30, float n31, float n32, float n33) noexcept;

    mat4(const vec4& a, const vec4& b, const vec4& c, const vec4& d) noexcept;

    float& operator()(int i, int j) noexcept;
    const float& operator()(int i, int j) const noexcept;

    vec4& operator[](int j) noexcept;
    const vec4& operator[](int j) const noexcept;

  protected:
    float n[4][4];
};

mat4 inverse(mat4 const& M) noexcept;

} // namespace wt
