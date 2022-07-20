// Some code is based on "Foundations of Game Engine Development, Volume 1: Mathematics", E.Lengyel

#include "mat3.hh"

namespace wt {

mat3 operator*(mat3 const& A, mat3 const& B) noexcept {
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

vec3 operator*(mat3 const& M, vec3 const& v) noexcept {
    return {M(0, 0) * v.x + M(0, 1) * v.y + M(0, 2) * v.z,
            M(1, 0) * v.x + M(1, 1) * v.y + M(1, 2) * v.z,
            M(2, 0) * v.x + M(2, 1) * v.y + M(2, 2) * v.z};
}

float det(mat3 const& M) noexcept {
    return M(0, 0) * (M(1, 1) * M(2, 2) - M(1, 2) * M(2, 1)) +
           M(0, 1) * (M(1, 2) * M(2, 0) - M(1, 0) * M(2, 2)) +
           M(0, 2) * (M(1, 0) * M(2, 1) - M(1, 1) * M(2, 0));
}

mat3 inverse(mat3 const& M) noexcept {
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

template <Axis a> mat3 rotation(float t) noexcept {
    float c = std::cos(t);
    float s = std::sin(t);

    if constexpr (a == Axis::X) {
        return {
            1.f, 0.f, 0.f, //
            0.f, c,   -s,  //
            0.f, s,   c,   //
        };
    } else if constexpr (a == Axis::Y) {
        return {
            c,   0.f, s,   //
            0.f, 1.f, 0.f, //
            -s,  0.f, c,   //
        };
    } else if constexpr (a == Axis::Z) {
        return {
            c,   -s,  0.f, //
            s,   c,   0.f, //
            0.f, 0.f, 1.f, //
        };
    }
    __builtin_unreachable();
}

template mat3 rotation<Axis::X>(float) noexcept;
template mat3 rotation<Axis::Y>(float) noexcept;
template mat3 rotation<Axis::Z>(float) noexcept;

mat3 rotation(float t, vec3 const& a) noexcept {
    float c = std::cos(t);
    float s = std::sin(t);
    float d = 1.f - c;

    float x = a.x * d;
    float y = a.y * d;
    float z = a.z * d;
    float axay = x * a.y;
    float axaz = x * a.z;
    float ayaz = y * a.z;

    return {
        c + x * a.x,    axay - s * a.z, axaz + s * a.y, //
        axay + s * a.z, c + y * a.y,    ayaz - s * a.x, //
        axaz - s * a.y, ayaz + s * a.x, c + z * a.z,    //
    };
}

mat3 reflection(vec3 const& a) noexcept {
    float x = a.x * -2.f;
    float y = a.y * -2.f;
    float z = a.z * -2.f;
    float axay = x * a.y;
    float axaz = x * a.z;
    float ayaz = y * a.z;

    return {
        x * a.x + 1.f, axay,          axaz,          //
        axay,          y * a.y + 1.f, ayaz,          //
        axaz,          ayaz,          z * a.z + 1.f, //
    };
}

mat3 involution(vec3 const& a) noexcept {
    float x = a.x * 2.f;
    float y = a.y * 2.f;
    float z = a.z * 2.f;
    float axay = x * a.y;
    float axaz = x * a.z;
    float ayaz = y * a.z;

    return {
        x * a.x - 1.f, axay,          axaz,          //
        axay,          y * a.y - 1.f, ayaz,          //
        axaz,          ayaz,          z * a.z - 1.f, //
    };
}

mat3 scale(float sx, float sy, float sz) noexcept {
    return {
        sx,  0.f, 0.f, //
        0.f, sy,  0.f, //
        0.f, 0.f, sz,  //
    };
}

mat3 scale(float s, vec3 const& a) noexcept {
    s -= 1.f;
    float x = a.x * s;
    float y = a.y * s;
    float z = a.z * s;
    float axay = x * a.y;
    float axaz = x * a.z;
    float ayaz = y * a.z;

    return {
        x * a.x + 1.f, axay,          axaz,          //
        axay,          y * a.y + 1.f, ayaz,          //
        axaz,          ayaz,          z * a.z + 1.f, //
    };
}

mat3 skew(float t, vec3 const& a, vec3 const& b) noexcept {
    t = std::tan(t);
    float x = a.x * t;
    float y = a.y * t;
    float z = a.z * t;

    return {
        x * b.x + 1.f, x * b.y,       x * b.z,       //
        y * b.x,       y * b.y + 1.f, y * b.z,       //
        z * b.x,       z * b.y,       z * b.z + 1.f, //
    };
}

} // namespace wt
