#pragma once

#include <cmath>
#include <cstddef>

namespace wt {

struct vec4 {
    float x, y, z, w;

    vec4() noexcept = default;
    vec4(float x, float y, float z, float w) noexcept : x{x}, y{y}, z{z}, w{w} {}

    float& operator[](std::size_t idx) noexcept {
        switch (idx) {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        case 3:
            return w;
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
        case 3:
            return w;
        default:
            __builtin_unreachable();
        }
    }

    vec4& operator*=(float s) noexcept {
        x *= s;
        y *= s;
        z *= s;
        w *= s;
        return *this;
    }

    vec4& operator/=(float s) noexcept {
        s = 1.0f / s;
        x *= s;
        y *= s;
        z *= s;
        w *= s;
        return *this;
    }

    vec4& operator+=(vec4 const& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        w += v.w;
        return *this;
    }

    vec4& operator-=(vec4 const& v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        w -= v.w;
        return *this;
    }
};

} // namespace wt
