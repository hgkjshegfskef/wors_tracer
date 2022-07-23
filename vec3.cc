#include "vec3.hh"

#include <fmt/format.h>

#include <cmath> // sqrt
#include <tuple> // tie

namespace wt {

vec3::vec3(float x, float y, float z) noexcept : x{x}, y{y}, z{z} {}

float& vec3::operator[](std::size_t idx) noexcept {
    switch (idx) {
    case 0:
        return x;
    case 1:
        return y;
    case 2:
        return z;
    default:
        __builtin_unreachable();
    }
}

float const& vec3::operator[](std::size_t idx) const noexcept {
    switch (idx) {
    case 0:
        return x;
    case 1:
        return y;
    case 2:
        return z;
    default:
        __builtin_unreachable();
    }
}

vec3& vec3::operator*=(float s) noexcept {
    x *= s;
    y *= s;
    z *= s;
    return *this;
}

vec3& vec3::operator/=(float s) noexcept {
    s = 1.0f / s;
    x *= s;
    y *= s;
    z *= s;
    return *this;
}

vec3& vec3::operator+=(const vec3& v) noexcept {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

vec3& vec3::operator-=(const vec3& v) noexcept {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

bool operator==(vec3 const& l, vec3 const& r) noexcept {
    return std::tie(l.x, l.y, l.z) == std::tie(r.x, r.y, r.z);
}

vec3 operator*(const vec3& v, float s) noexcept { return {v.x * s, v.y * s, v.z * s}; }
vec3 operator*(float s, const vec3& v) noexcept { return v * s; }
vec3 operator/(const vec3& v, float s) noexcept {
    s = 1.0f / s;
    return {v.x * s, v.y * s, v.z * s};
}

vec3 operator-(const vec3& v) noexcept { return {-v.x, -v.y, -v.z}; }
vec3 operator+(const vec3& a, const vec3& b) noexcept { return {a.x + b.x, a.y + b.y, a.z + b.z}; }
vec3 operator-(const vec3& a, const vec3& b) noexcept { return {a.x - b.x, a.y - b.y, a.z - b.z}; }

float magnitude(const vec3& v) noexcept { return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z); }

vec3 normalize(const vec3& v) noexcept { return v / magnitude(v); }

float dot(const vec3& a, const vec3& b) noexcept { return a.x * b.x + a.y * b.y + a.z * b.z; }

vec3 cross(const vec3& a, const vec3& b) noexcept {
    return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}

vec3 project(const vec3& a, const vec3& b, bool b_is_unit) noexcept {
    if (b_is_unit) {
        return dot(a, b) * b;
    }
    return (dot(a, b) / dot(b, b)) * b;
}

vec3 reject(const vec3& a, const vec3& b, bool b_is_unit) noexcept {
    if (b_is_unit) {
        return a - dot(a, b) * b;
    }
    return a - (dot(a, b) / dot(b, b)) * b;
}

} // namespace wt

auto fmt::formatter<wt::vec3>::format(const wt::vec3& vec, fmt::format_context& ctx)
    -> decltype(ctx.out()) {
    auto&& out = ctx.out();
    fmt::format_to(out, "[{}, {}, {}]ᵀ", vec.x, vec.y, vec.z);
    return out;
}

template struct fmt::formatter<wt::vec3>;
