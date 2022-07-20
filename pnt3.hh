// Some code is based on "Foundations of Game Engine Development, Volume 1: Mathematics", E.Lengyel

#pragma once

#include "vec3.hh"

namespace wt {

struct pnt3 final : vec3 {
    pnt3() noexcept = default;
    pnt3(float a, float b, float c) noexcept : vec3(a, b, c) {}
    pnt3(vec3 const& v) noexcept : vec3(v) {}

    pnt3& operator=(vec3 const& v) noexcept {
        x = v.x;
        y = v.y;
        z = v.z;
        return *this;
    }
};

inline pnt3 operator+(pnt3 const& a, vec3 const& b) noexcept {
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

inline pnt3 operator+(vec3 const& b, pnt3 const& a) noexcept { return a + b; }

inline pnt3 operator-(pnt3 const& a, vec3 const& b) noexcept {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

inline vec3 operator-(pnt3 const& a, pnt3 const& b) noexcept {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

} // namespace wt
