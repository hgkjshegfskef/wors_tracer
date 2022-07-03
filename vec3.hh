// Some code is based on "Foundations of Game Engine Development, Volume 1: Mathematics", E.Lengyel

#pragma once

#include <cmath>
#include <cstddef>

namespace wt {

struct vec3 {
    float x, y, z;

    vec3() noexcept = default;
    vec3(float x, float y, float z) noexcept : x{x}, y{y}, z{z} {}

    float& operator[](std::size_t idx) noexcept {
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

    float const& operator[](std::size_t idx) const noexcept {
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

    vec3& operator*=(float s) noexcept {
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }

    vec3& operator/=(float s) noexcept {
        s = 1.0f / s;
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }

    vec3& operator+=(vec3 const& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    vec3& operator-=(vec3 const& v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }
};

inline vec3 operator*(vec3 const& v, float s) noexcept { return {v.x * s, v.y * s, v.z * s}; }

inline vec3 operator*(float s, vec3 const& v) noexcept { return {v.x * s, v.y * s, v.z * s}; }

inline vec3 operator/(vec3 const& v, float s) noexcept {
    s = 1.0f / s;
    return {v.x * s, v.y * s, v.z * s};
}

inline vec3 operator-(vec3 const& v) noexcept { return {-v.x, -v.y, -v.z}; }

inline vec3 operator+(vec3 const& a, vec3 const& b) { return {a.x + b.x, a.y + b.y, a.z + b.z}; }

inline vec3 operator-(vec3 const& a, vec3 const& b) { return {a.x - b.x, a.y - b.y, a.z - b.z}; }

inline float magnitude(vec3 const& v) noexcept {
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

inline vec3 normalize(vec3 const& v) noexcept { return v / magnitude(v); }

inline float dot(vec3 const& a, vec3 const& b) noexcept {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline vec3 cross(vec3 const& a, vec3 const& b) noexcept {
    return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}

inline vec3 project(vec3 const& a, vec3 const& b, bool b_is_unit = false) noexcept {
    if (b_is_unit) {
        return dot(a, b) * b;
    }
    return (dot(a, b) / dot(b, b)) * b;
}

inline vec3 reject(vec3 const& a, vec3 const& b, bool b_is_unit = false) noexcept {
    if (b_is_unit) {
        return a - dot(a, b) * b;
    }
    return a - (dot(a, b) / dot(b, b)) * b;
}

} // namespace wt
