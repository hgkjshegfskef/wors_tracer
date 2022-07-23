#include "tform4.hh"

namespace wt {

tform4::tform4(float n00, float n01, float n02, float n03, //
               float n10, float n11, float n12, float n13, //
               float n20, float n21, float n22, float n23) noexcept {
    // clang-format off
    n[0][0] = n00; n[0][1] = n10; n[0][2] = n20;
    n[1][0] = n01; n[1][1] = n11; n[1][2] = n21;
    n[2][0] = n02; n[2][1] = n12; n[2][2] = n22;
    n[3][0] = n03; n[3][1] = n13; n[3][2] = n23;
    // clang-format on

    n[0][3] = n[1][3] = n[2][3] = 0.f;
    n[3][3] = 1.f;
}

tform4::tform4(vec3 const& a, vec3 const& b, vec3 const& c, pnt3 const& p) noexcept {
    // clang-format off
    n[0][0] = a.x; n[0][1] = a.y; n[0][2] = a.z; n[1][0] = b.x;
    n[1][1] = b.y; n[1][2] = b.z; n[2][0] = c.x; n[2][1] = c.y;
    n[2][2] = c.z; n[3][0] = p.x; n[3][1] = p.y; n[3][2] = p.z;
    // clang-format on

    n[0][3] = n[1][3] = n[2][3] = 0.f;
    n[3][3] = 1.f;
}

pnt3 const& tform4::get_translation() const noexcept {
    return *reinterpret_cast<pnt3 const*>(n[3]); // UB
}

void tform4::set_translation(const pnt3& p) noexcept {
    n[3][0] = p.x;
    n[3][1] = p.y;
    n[3][2] = p.z;
}

vec3& tform4::operator[](int j) noexcept {
    return *reinterpret_cast<vec3*>(n[j]); // UB
}

vec3 const& tform4::operator[](int j) const noexcept {
    return *reinterpret_cast<vec3 const*>(n[j]); // UB
}

tform4 tform4::identity() noexcept {
    return {1.f, 0.f, 0.f, 0.f, //
            0.f, 1.f, 0.f, 0.f, //
            0.f, 0.f, 1.f, 0.f};
}

tform4 inverse(const tform4& H) noexcept {
    vec3 const& a = H[0];
    vec3 const& b = H[1];
    vec3 const& c = H[2];
    vec3 const& d = H[3];

    vec3 s = cross(a, b);
    vec3 t = cross(c, d);

    float inv_det = 1.f / dot(s, c);

    s *= inv_det;
    t *= inv_det;
    vec3 v = c * inv_det;

    vec3 r0 = cross(b, v);
    vec3 r1 = cross(v, a);

    return {r0.x, r0.y, r0.z, -dot(b, t), //
            r1.x, r1.y, r1.z, dot(a, t),  //
            s.x,  s.y,  s.z,  -dot(d, s)};
}

tform4 operator*(const tform4& A, const tform4& B) noexcept {
    return {A(0, 0) * B(0, 0) + A(0, 1) * B(1, 0) + A(0, 2) * B(2, 0),           //
            A(0, 0) * B(0, 1) + A(0, 1) * B(1, 1) + A(0, 2) * B(2, 1),           //
            A(0, 0) * B(0, 2) + A(0, 1) * B(1, 2) + A(0, 2) * B(2, 2),           //
            A(0, 0) * B(0, 3) + A(0, 1) * B(1, 3) + A(0, 2) * B(2, 3) + A(0, 3), //
            A(1, 0) * B(0, 0) + A(1, 1) * B(1, 0) + A(1, 2) * B(2, 0),           //
            A(1, 0) * B(0, 1) + A(1, 1) * B(1, 1) + A(1, 2) * B(2, 1),           //
            A(1, 0) * B(0, 2) + A(1, 1) * B(1, 2) + A(1, 2) * B(2, 2),           //
            A(1, 0) * B(0, 3) + A(1, 1) * B(1, 3) + A(1, 2) * B(2, 3) + A(1, 3), //
            A(2, 0) * B(0, 0) + A(2, 1) * B(1, 0) + A(2, 2) * B(2, 0),           //
            A(2, 0) * B(0, 1) + A(2, 1) * B(1, 1) + A(2, 2) * B(2, 1),           //
            A(2, 0) * B(0, 2) + A(2, 1) * B(1, 2) + A(2, 2) * B(2, 2),           //
            A(2, 0) * B(0, 3) + A(2, 1) * B(1, 3) + A(2, 2) * B(2, 3) + A(2, 3)};
}

vec3 operator*(const tform4& H, const vec3& v) noexcept {
    return {H(0, 0) * v.x + H(0, 1) * v.y + H(0, 2) * v.z, //
            H(1, 0) * v.x + H(1, 1) * v.y + H(1, 2) * v.z, //
            H(2, 0) * v.x + H(2, 1) * v.y + H(2, 2) * v.z};
}

pnt3 operator*(const tform4& H, const pnt3& p) noexcept {
    return {H(0, 0) * p.x + H(0, 1) * p.y + H(0, 2) * p.z + H(0, 3), //
            H(1, 0) * p.x + H(1, 1) * p.y + H(1, 2) * p.z + H(1, 3), //
            H(2, 0) * p.x + H(2, 1) * p.y + H(2, 2) * p.z + H(2, 3)};
}

vec3 operator*(const vec3& n, const tform4& H) noexcept {
    return {n.x * H(0, 0) + n.y * H(1, 0) + n.z * H(2, 0), //
            n.x * H(0, 1) + n.y * H(1, 1) + n.z * H(2, 1), //
            n.x * H(0, 2) + n.y * H(1, 2) + n.z * H(2, 2)};
}

} // namespace wt
