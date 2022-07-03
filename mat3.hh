// Some code is based on "Foundations of Game Engine Development, Volume 1: Mathematics", E.Lengyel

#pragma once

#include "vec3.hh"

namespace wt {

class mat3 {
  public:
    mat3() noexcept = default;

    // clang-format off

    mat3(float n00, float n01, float n02,
         float n10, float n11, float n12,
         float n20, float n21, float n22) noexcept {
        n[0][0] = n00; n[0][1] = n10; n[0][2] = n20;
        n[1][0] = n01; n[1][1] = n11; n[1][2] = n21;
        n[2][0] = n02; n[2][1] = n12; n[2][2] = n22;
    }

    mat3(vec3 const& a, vec3 const& b, vec3 const& c) noexcept {
        n[0][0] = a.x; n[0][1] = a.y; n[0][2] = a.z;
        n[1][0] = b.x; n[1][1] = b.y; n[1][2] = b.z;
        n[2][0] = c.x; n[2][1] = c.y; n[2][2] = c.z;
    }

    // clang-format on

    float& operator()(int i, int j) noexcept { return n[j][i]; }

    float const& operator()(int i, int j) const noexcept { return n[j][i]; }

    vec3& operator[](int j) noexcept { return /*UB*/ *reinterpret_cast<vec3*>(n[j]); }

    //    vec3 operator[](int j) noexcept { return {n[j][0], n[j][1], n[j][2]}; }

    vec3 const& operator[](int j) const noexcept {
        return /*UB*/ *reinterpret_cast<vec3 const*>(n[j]);
    }

    //    vec3 const operator[](int j) const noexcept { return {n[j][0], n[j][1], n[j][2]}; }

  private:
    // Column-major
    float n[3][3];
};

inline mat3 operator*(mat3 const& A, mat3 const& B) noexcept {
    return {A(0, 0) * B(0, 0) + A(0, 1) * B(1, 0) + A(0, 2) * B(2, 0),
            A(0, 0) * B(0, 1) + A(0, 1) * B(1, 1) + A(0, 2) * B(2, 1),
            A(0, 0) * B(0, 2) + A(0, 1) * B(1, 2) + A(0, 2) * B(2, 2),
            A(1, 0) * B(0, 0) + A(1, 1) * B(1, 0) + A(1, 2) * B(2, 0),
            A(1, 0) * B(0, 1) + A(1, 1) * B(1, 1) + A(1, 2) * B(2, 1),
            A(1, 0) * B(0, 2) + A(1, 1) * B(1, 2) + A(1, 2) * B(2, 2),
            A(2, 0) * B(0, 0) + A(2, 1) * B(1, 0) + A(2, 2) * B(2, 0),
            A(2, 0) * B(0, 1) + A(2, 1) * B(1, 1) + A(2, 2) * B(2, 1),
            A(2, 0) * B(0, 2) + A(2, 1) * B(1, 2) + A(2, 2) * B(2, 2)};
}

inline vec3 operator*(mat3 const& M, vec3 const& v) noexcept {
    return {M(0, 0) * v.x + M(0, 1) * v.y + M(0, 2) * v.z,
            M(1, 0) * v.x + M(1, 1) * v.y + M(1, 2) * v.z,
            M(2, 0) * v.x + M(2, 1) * v.y + M(2, 2) * v.z};
}

inline float det(mat3 const& M) noexcept {
    return M(0, 0) * (M(1, 1) * M(2, 2) - M(1, 2) * M(2, 1)) +
           M(0, 1) * (M(1, 2) * M(2, 0) - M(1, 0) * M(2, 2)) +
           M(0, 2) * (M(1, 0) * M(2, 1) - M(1, 1) * M(2, 0));
}

inline mat3 inverse(mat3 const& M) {
    vec3 const& a = M[0];
    vec3 const& b = M[1];
    vec3 const& c = M[2];

    vec3 r0 = cross(b, c);
    vec3 r1 = cross(c, a);
    vec3 r2 = cross(a, b);

    float inv_det = 1.f / dot(r2, c);

    return {r0.x * inv_det, r0.y * inv_det, r0.z * inv_det, //
            r1.x * inv_det, r1.y * inv_det, r1.z * inv_det, //
            r2.x * inv_det, r2.y * inv_det, r2.z * inv_det};
}

} // namespace wt
