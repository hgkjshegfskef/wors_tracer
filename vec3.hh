#pragma once

#include <cstddef>

namespace wt {

struct vec3 {
    float x, y, z;

    vec3() noexcept = default;
    vec3(float x, float y, float z) noexcept : x{x}, y{y}, z{z} {}

    float& operator[](std::size_t idx) noexcept { return (&x)[idx]; }             // UB
    float const& operator[](std::size_t idx) const noexcept { return (&x)[idx]; } // UB
};

} // namespace wt
