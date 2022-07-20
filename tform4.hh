// Some code is based on "Foundations of Game Engine Development, Volume 1: Mathematics", E.Lengyel

#pragma once

#include "mat4.hh"
#include "pnt3.hh"
#include "vec3.hh"

namespace wt {

class tform4 final : public mat4 {
  public:
    tform4() noexcept = default;

    // clang-format off

    tform4(float n00, float n01, float n02, float n03,
           float n10, float n11, float n12, float n13,
           float n20, float n21, float n22, float n23) noexcept {
        n[0][0] = n00; n[0][1] = n10; n[0][2] = n20;
        n[1][0] = n01; n[1][1] = n11; n[1][2] = n21;
        n[2][0] = n02; n[2][1] = n12; n[2][2] = n22;
        n[3][0] = n03; n[3][1] = n13; n[3][2] = n23;

        n[0][3] = n[1][3] = n[2][3] = 0.f;
        n[3][3] = 1.f;
    }

    tform4(vec3 const& a, vec3 const& b, vec3 const& c, pnt3 const& p) noexcept {
        n[0][0] = a.x; n[0][1] = a.y; n[0][2] = a.z; n[1][0] = b.x;
        n[1][1] = b.y; n[1][2] = b.z; n[2][0] = c.x; n[2][1] = c.y;
        n[2][2] = c.z; n[3][0] = p.x; n[3][1] = p.y; n[3][2] = p.z;

        n[0][3] = n[1][3] = n[2][3] = 0.f;
        n[3][3] = 1.f;
    }

    // clang-format on

    pnt3 const& get_translation() const noexcept {
        return *reinterpret_cast<pnt3 const*>(n[3]); // UB
    }

    void set_translation(pnt3 const& p) noexcept {
        n[3][0] = p.x;
        n[3][1] = p.y;
        n[3][2] = p.z;
    }

    vec3& operator[](int j) noexcept {
        return *reinterpret_cast<vec3*>(n[j]); // UB
    }

    vec3 const& operator[](int j) const noexcept {
        return *reinterpret_cast<vec3 const*>(n[j]); // UB
    }
};

tform4 inverse(tform4 const& H) noexcept;

tform4 operator*(tform4 const& A, tform4 const& B) noexcept;

// transform column vector
vec3 operator*(tform4 const& H, vec3 const& v) noexcept;

// transform position vector
pnt3 operator*(tform4 const& H, pnt3 const& p) noexcept;

// transform normal vector (as row) from coordinate system B to coordinate system A: nM^-1, where M
// is the upper left 3x3 of H.
// Therefore, H is assumed to be inverted.
// If upper-left 3x3 of H is orthogonal, normal vector can be transformed just like regular vector: Mn.
vec3 operator*(vec3 const& n, tform4 const& H) noexcept;

} // namespace wt
