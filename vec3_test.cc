#include "vec3.hh"

#include <gtest/gtest.h>

#include <cmath>

using namespace wt;

TEST(vec3test, DefaultCtor) { [[maybe_unused]] vec3 v; }

TEST(vec3test, UserDefinedCtor) { [[maybe_unused]] vec3 v(1.f, 2.f, 3.f); }

TEST(vec3test, Members) {
    vec3 v(1.f, 2.f, 3.f);

    EXPECT_NEAR(v.x, 1.f, 1e-6);
    EXPECT_NEAR(v.y, 2.f, 1e-6);
    EXPECT_NEAR(v.z, 3.f, 1e-6);
}

TEST(vec3test, SubscriptOperator) {
    vec3 v(1.f, 2.f, 3.f);

    EXPECT_NEAR(v[0], 1.f, 1e-6);
    EXPECT_NEAR(v[1], 2.f, 1e-6);
    EXPECT_NEAR(v[2], 3.f, 1e-6);
}

TEST(vec3test, ConstSubscriptOperator) {
    vec3 const v(1.f, 2.f, 3.f);

    EXPECT_NEAR(v[0], 1.f, 1e-6);
    EXPECT_NEAR(v[1], 2.f, 1e-6);
    EXPECT_NEAR(v[2], 3.f, 1e-6);
}

TEST(vec3test, MulEqScalarOperator) {
    vec3 v(1.f, 2.f, 3.f);
    float s = 2.f;
    v *= s;

    EXPECT_NEAR(v.x, 2.f, 1e-6);
    EXPECT_NEAR(v.y, 4.f, 1e-6);
    EXPECT_NEAR(v.z, 6.f, 1e-6);
}

TEST(vec3test, DivEqScalarOperator) {
    vec3 v(2.f, 4.f, 6.f);
    float s = 2.f;
    v /= s;

    EXPECT_NEAR(v.x, 1.f, 1e-6);
    EXPECT_NEAR(v.y, 2.f, 1e-6);
    EXPECT_NEAR(v.z, 3.f, 1e-6);
}

TEST(vec3test, MulScalar) {
    vec3 v(1.f, 2.f, 3.f);
    float s = 2.f;
    auto v1 = v * s;
    auto v2 = s * v;

    EXPECT_NEAR(v1.x, 2.f, 1e-6);
    EXPECT_NEAR(v1.y, 4.f, 1e-6);
    EXPECT_NEAR(v1.z, 6.f, 1e-6);
    EXPECT_NEAR(v2.x, 2.f, 1e-6);
    EXPECT_NEAR(v2.y, 4.f, 1e-6);
    EXPECT_NEAR(v2.z, 6.f, 1e-6);
}

TEST(vec3test, DivScalar) {
    vec3 v(2.f, 4.f, 6.f);
    float s = 2.f;
    auto v1 = v / s;

    EXPECT_NEAR(v1.x, 1.f, 1e-6);
    EXPECT_NEAR(v1.y, 2.f, 1e-6);
    EXPECT_NEAR(v1.z, 3.f, 1e-6);
}

TEST(vec3test, Negate) {
    vec3 v(1.f, 2.f, 3.f);
    auto v1 = -v;

    EXPECT_NEAR(v1.x, -1.f, 1e-6);
    EXPECT_NEAR(v1.y, -2.f, 1e-6);
    EXPECT_NEAR(v1.z, -3.f, 1e-6);
}

TEST(vec3test, Normalize) {
    vec3 v(5.f, 6.f, 10.f);
    auto result = normalize(v);

    EXPECT_NEAR(result.x, .394055f, 1e-6);
    EXPECT_NEAR(result.y, .472866f, 1e-6);
    EXPECT_NEAR(result.z, .788110f, 1e-6);
}

TEST(vec3test, NormalizeZero) {
    vec3 v(0.f, 0.f, 0.f);
    auto result = normalize(v);

    EXPECT_TRUE(std::isnan(result.x));
    EXPECT_TRUE(std::isnan(result.y));
    EXPECT_TRUE(std::isnan(result.z));
}
