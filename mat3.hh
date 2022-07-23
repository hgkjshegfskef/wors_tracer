// Some code is based on "Foundations of Game Engine Development, Volume 1: Mathematics", E.Lengyel

#pragma once

#include "vec3.hh"

namespace wt {

class mat3 final {
  public:
    mat3() noexcept = default;

    mat3(float n00, float n01, float n02, //
         float n10, float n11, float n12, //
         float n20, float n21, float n22) noexcept;

    mat3(vec3 const& a, vec3 const& b, vec3 const& c) noexcept;

    float& operator()(int i, int j) noexcept;
    float const& operator()(int i, int j) const noexcept;

    vec3& operator[](int j) noexcept;
    vec3 const& operator[](int j) const noexcept;

  private:
    // Column-major
    float n[3][3];
};

mat3 operator*(mat3 const& A, mat3 const& B) noexcept;
vec3 operator*(mat3 const& M, vec3 const& v) noexcept;

float det(mat3 const& M) noexcept;
mat3 inverse(mat3 const& M) noexcept;

enum class Axis : unsigned char { X, Y, Z };
// Rotation around standard axis by angle t (rad)
template <Axis a> mat3 rotation(float t) noexcept;
extern template mat3 rotation<Axis::X>(float) noexcept;
extern template mat3 rotation<Axis::Y>(float) noexcept;
extern template mat3 rotation<Axis::Z>(float) noexcept;
// Rotation around arbitrary axis a (unit len)
mat3 rotation(float t, vec3 const& a) noexcept;

// Reflection through plane perpendicular to a (unit len)
mat3 reflection(vec3 const& a) noexcept;
// Involution through a (unit len)
mat3 involution(vec3 const& a) noexcept;

// Nonuniform scale
mat3 scale(float sx, float sy, float sz) noexcept;
// Uniform scale by s along direction a (unit len)
mat3 scale(float s, vec3 const& a) noexcept;

// Skew by angle t (rad) along a based on projected len along b (both orthog and unit len)
mat3 skew(float t, vec3 const& a, vec3 const& b) noexcept;

} // namespace wt
