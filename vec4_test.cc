#include "vec4.hh"

#include <gtest/gtest.h>

#include <cmath>

using namespace wt;

TEST(vec4test, DefaultCtor) { [[maybe_unused]] vec4 v; }

TEST(vec4test, UserDefinedCtor) { [[maybe_unused]] vec4 v(1.f, 2.f, 3.f, 4.f); }

TEST(vec4test, Members) {
    vec4 v(1.f, 2.f, 3.f, 4.f);

    EXPECT_NEAR(v.x, 1.f, 1e-6);
    EXPECT_NEAR(v.y, 2.f, 1e-6);
    EXPECT_NEAR(v.z, 3.f, 1e-6);
    EXPECT_NEAR(v.w, 4.f, 1e-6);
}

TEST(vec4test, SubscriptOperator) {
    vec4 v(1.f, 2.f, 3.f, 4.f);

    EXPECT_NEAR(v[0], 1.f, 1e-6);
    EXPECT_NEAR(v[1], 2.f, 1e-6);
    EXPECT_NEAR(v[2], 3.f, 1e-6);
    EXPECT_NEAR(v[3], 4.f, 1e-6);
}

TEST(vec4test, ConstSubscriptOperator) {
    vec4 const v(1.f, 2.f, 3.f, 4.f);

    EXPECT_NEAR(v[0], 1.f, 1e-6);
    EXPECT_NEAR(v[1], 2.f, 1e-6);
    EXPECT_NEAR(v[2], 3.f, 1e-6);
    EXPECT_NEAR(v[3], 4.f, 1e-6);
}

TEST(vec4test, MulEqScalarOperator) {
    vec4 v(1.f, 2.f, 3.f, 4.f);
    float s = 2.f;
    v *= s;

    EXPECT_NEAR(v.x, 2.f, 1e-6);
    EXPECT_NEAR(v.y, 4.f, 1e-6);
    EXPECT_NEAR(v.z, 6.f, 1e-6);
    EXPECT_NEAR(v.w, 8.f, 1e-6);
}

TEST(vec4test, DivEqScalarOperator) {
    vec4 v(2.f, 4.f, 6.f, 8.f);
    float s = 2.f;
    v /= s;

    EXPECT_NEAR(v.x, 1.f, 1e-6);
    EXPECT_NEAR(v.y, 2.f, 1e-6);
    EXPECT_NEAR(v.z, 3.f, 1e-6);
    EXPECT_NEAR(v.w, 4.f, 1e-6);
}
