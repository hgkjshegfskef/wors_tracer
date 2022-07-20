// Some code is based on "Foundations of Game Engine Development, Volume 1: Mathematics", E.Lengyel

#pragma once

#include "vec3.hh"
#include "vec4.hh"

namespace wt {

class mat4 {
  public:
    mat4() noexcept = default;

    // clang-format off

    mat4(float n00, float n01, float n02, float n03,
         float n10, float n11, float n12, float n13,
         float n20, float n21, float n22, float n23,
         float n30, float n31, float n32, float n33) noexcept {
        n[0][0] = n00; n[0][1] = n10; n[0][2] = n20; n[0][3] = n30;
        n[1][0] = n01; n[1][1] = n11; n[1][2] = n21; n[1][3] = n31;
        n[2][0] = n02; n[2][1] = n12; n[2][2] = n22; n[2][3] = n32;
        n[3][0] = n03; n[3][1] = n13; n[3][2] = n23; n[3][3] = n33;
    }

    mat4(const vec4& a, const vec4& b, const vec4& c, const vec4& d) noexcept {
        n[0][0] = a.x; n[0][1] = a.y; n[0][2] = a.z; n[0][3] = a.w;
        n[1][0] = b.x; n[1][1] = b.y; n[1][2] = b.z; n[1][3] = b.w;
        n[2][0] = c.x; n[2][1] = c.y; n[2][2] = c.z; n[2][3] = c.w;
        n[3][0] = d.x; n[3][1] = d.y; n[3][2] = d.z; n[3][3] = d.w;
    }

    // clang-format on

    float& operator()(int i, int j) noexcept { return n[j][i]; }

    const float& operator()(int i, int j) const noexcept { return n[j][i]; }

    vec4& operator[](int j) noexcept {
        return *reinterpret_cast<vec4*>(n[j]); // UB
    }

    const vec4& operator[](int j) const noexcept {
        return *reinterpret_cast<vec4 const*>(n[j]); // UB
    }

  protected:
    float n[4][4];
};

inline mat4 inverse(mat4 const& M) noexcept {
    vec3 const& a = reinterpret_cast<const vec3&>(M[0]); // UB
    vec3 const& b = reinterpret_cast<const vec3&>(M[1]); // UB
    vec3 const& c = reinterpret_cast<const vec3&>(M[2]); // UB
    vec3 const& d = reinterpret_cast<const vec3&>(M[3]); // UB

    const float& x = M(3, 0);
    const float& y = M(3, 1);
    const float& z = M(3, 2);
    const float& w = M(3, 3);

    vec3 s = cross(a, b);
    vec3 t = cross(c, d);
    vec3 u = a * y - b * x;
    vec3 v = c * w - d * z;

    float inv_det = 1.f / (dot(s, v) + dot(t, u));
    s *= inv_det;
    t *= inv_det;
    u *= inv_det;
    v *= inv_det;

    vec3 r0 = cross(b, v) + t * y;
    vec3 r1 = cross(v, a) - t * x;
    vec3 r2 = cross(d, u) + s * w;
    vec3 r3 = cross(u, c) - s * z;

    return {r0.x, r0.y, r0.z, -dot(b, t), //
            r1.x, r1.y, r1.z, dot(a, t),  //
            r2.x, r2.y, r2.z, -dot(d, s), //
            r3.x, r3.y, r3.z, dot(c, s)};
}

} // namespace wt
