// Some code is based on "Foundations of Game Engine Development, Volume 1: Mathematics", E.Lengyel

#pragma once

#include "vec3.hh"

#include <fmt/core.h>

namespace wt {

struct pnt3 final : vec3 {
    pnt3() noexcept = default;
    pnt3(float x, float y, float z) noexcept;
    pnt3(vec3 const& v) noexcept;

    pnt3& operator=(vec3 const& v) noexcept;
};

pnt3 operator+(pnt3 const& a, vec3 const& b) noexcept;
pnt3 operator+(vec3 const& b, pnt3 const& a) noexcept;
pnt3 operator-(pnt3 const& a, vec3 const& b) noexcept;
vec3 operator-(pnt3 const& a, pnt3 const& b) noexcept;

} // namespace wt

template <> struct fmt::formatter<wt::pnt3> : fmt::formatter<float> {
    auto format(wt::pnt3 const& pnt, fmt::format_context& ctx) -> decltype(ctx.out());
};

extern template struct fmt::formatter<wt::pnt3>;
