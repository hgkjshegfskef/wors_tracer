#include "pnt3.hh"

#include <fmt/format.h>

namespace wt {

pnt3::pnt3(float x, float y, float z) noexcept : vec3(x, y, z) {}
pnt3::pnt3(const vec3& v) noexcept : vec3(v) {}

pnt3& pnt3::operator=(const vec3& v) noexcept {
    x = v.x;
    y = v.y;
    z = v.z;
    return *this;
}

pnt3 operator+(const pnt3& a, const vec3& b) noexcept { return {a.x + b.x, a.y + b.y, a.z + b.z}; }
pnt3 operator+(const vec3& b, const pnt3& a) noexcept { return a + b; }
pnt3 operator-(const pnt3& a, const vec3& b) noexcept { return {a.x - b.x, a.y - b.y, a.z - b.z}; }
vec3 operator-(const pnt3& a, const pnt3& b) noexcept { return {a.x - b.x, a.y - b.y, a.z - b.z}; }

} // namespace wt

auto fmt::formatter<wt::pnt3>::format(const wt::pnt3& pnt, fmt::format_context& ctx)
    -> decltype(ctx.out()) {
    auto&& out = ctx.out();
    fmt::format_to(out, "({}, {}, {})", pnt.x, pnt.y, pnt.z);
    return out;
}

template struct fmt::formatter<wt::pnt3>;
