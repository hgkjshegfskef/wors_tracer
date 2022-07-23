#include "pnt4.hh"

namespace wt {

pnt4::pnt4(float a, float b, float c, float d) noexcept : vec4(a, b, c, d) {}
pnt4::pnt4(const vec4& v) noexcept : vec4(v) { w = 1.f; }

pnt4& pnt4::operator=(const vec4& v) noexcept {
    x = v.x;
    y = v.y;
    z = v.z;
    w = 1.f;
    return *this;
}

pnt4 operator+(const pnt4& a, const vec4& b) noexcept {
    return {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}

pnt4 operator+(const vec4& b, const pnt4& a) noexcept { return a + b; }

pnt4 operator-(const pnt4& a, const vec4& b) noexcept {
    return {a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}

vec4 operator-(const pnt4& a, const pnt4& b) noexcept {
    return {a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}

} // namespace wt
