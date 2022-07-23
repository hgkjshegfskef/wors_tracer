// Some code is based on "Foundations of Game Engine Development, Volume 1: Mathematics", E.Lengyel

#pragma once

#include <cstddef>

namespace wt {

struct vec4 {
    float x, y, z, w;

    vec4() noexcept = default;
    vec4(float x, float y, float z, float w = 0.f) noexcept;

    float& operator[](std::size_t idx) noexcept;
    float const& operator[](std::size_t idx) const noexcept;

    vec4& operator*=(float s) noexcept;
    vec4& operator/=(float s) noexcept;

    vec4& operator+=(vec4 const& v);
    vec4& operator-=(vec4 const& v);
};

} // namespace wt
