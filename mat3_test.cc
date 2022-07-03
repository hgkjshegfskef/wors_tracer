#include "mat3.hh"

#include <gtest/gtest.h>

using namespace wt;

TEST(mat3test, DefaultCtor) { [[maybe_unused]] mat3 m; }

TEST(mat3test, ColumnVec) {
    mat3 m(vec3(1.f, 2.f, 3.f), vec3(4.f, 5.f, 6.f), vec3(7.f, 8.f, 9.f));
    vec3 const& v0 = m[0];
    vec3 const& v1 = m[1];
    vec3 const& v2 = m[2];

    EXPECT_NEAR(v0.x, 1.f, 1e-6);
    EXPECT_NEAR(v0.y, 2.f, 1e-6);
    EXPECT_NEAR(v0.z, 3.f, 1e-6);

    EXPECT_NEAR(v1.x, 4.f, 1e-6);
    EXPECT_NEAR(v1.y, 5.f, 1e-6);
    EXPECT_NEAR(v1.z, 6.f, 1e-6);

    EXPECT_NEAR(v2.x, 7.f, 1e-6);
    EXPECT_NEAR(v2.y, 8.f, 1e-6);
    EXPECT_NEAR(v2.z, 9.f, 1e-6);
}
