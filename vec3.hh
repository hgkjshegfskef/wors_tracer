// Some code is based on "Foundations of Game Engine Development, Volume 1: Mathematics", E.Lengyel

#pragma once

#include <fmt/core.h>

#include <cstddef> // size_t

namespace wt {

struct vec3 {
    float x, y, z;

    vec3() noexcept = default;
    vec3(float x, float y, float z) noexcept;

    float& operator[](std::size_t idx) noexcept;

    float const& operator[](std::size_t idx) const noexcept;

    vec3& operator*=(float s) noexcept;
    vec3& operator/=(float s) noexcept;

    vec3& operator+=(vec3 const& v) noexcept;
    vec3& operator-=(vec3 const& v) noexcept;
};

bool operator==(vec3 const& l, vec3 const& r) noexcept;

vec3 operator*(vec3 const& v, float s) noexcept;
vec3 operator*(float s, vec3 const& v) noexcept;
vec3 operator/(vec3 const& v, float s) noexcept;

vec3 operator-(vec3 const& v) noexcept;
vec3 operator+(vec3 const& a, vec3 const& b) noexcept;
vec3 operator-(vec3 const& a, vec3 const& b) noexcept;

float magnitude(vec3 const& v) noexcept;
vec3 normalize(vec3 const& v) noexcept;

float dot(vec3 const& a, vec3 const& b) noexcept;
vec3 cross(vec3 const& a, vec3 const& b) noexcept;

// Projection of a onto b
vec3 project(vec3 const& a, vec3 const& b, bool b_is_unit = false) noexcept;
// Rejection of a from b
vec3 reject(vec3 const& a, vec3 const& b, bool b_is_unit = false) noexcept;

} // namespace wt

template <> struct fmt::formatter<wt::vec3> : fmt::formatter<float> {
    auto format(wt::vec3 const& vec, fmt::format_context& ctx) -> decltype(ctx.out());
};

extern template struct fmt::formatter<wt::vec3>;
