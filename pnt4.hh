// Some code is based on "Foundations of Game Engine Development, Volume 1: Mathematics", E.Lengyel

#pragma once

#include "vec4.hh"

namespace wt {

struct pnt4 final : vec4 {
    pnt4() noexcept = default;
    pnt4(float a, float b, float c, float d = 1.f) noexcept;
    pnt4(vec4 const& v) noexcept;

    pnt4& operator=(vec4 const& v) noexcept;
};

pnt4 operator+(pnt4 const& a, vec4 const& b) noexcept;
pnt4 operator+(vec4 const& b, pnt4 const& a) noexcept;
pnt4 operator-(pnt4 const& a, vec4 const& b) noexcept;
vec4 operator-(pnt4 const& a, pnt4 const& b) noexcept;

} // namespace wt
