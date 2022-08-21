#include "mat3.hh"
#include "pnt3.hh"
#include "tform4.hh"

#include <gtest/gtest.h>

#include <numbers>

using namespace wt;
using namespace std::numbers;

TEST(TformTest, Translating) {
    pnt3 p1{-3, 4, 5};
    tform4 t;
    t.set_translation(pnt3{5, -3, 2});
    pnt3 p2 = t * p1;
    EXPECT_NEAR(p2.x, 2, 1e-6);
    EXPECT_NEAR(p2.y, 1, 1e-6);
    EXPECT_NEAR(p2.z, 7, 1e-6);
}

TEST(TformTest, Scaling) {
    vec3 v1{-4, 6, 8};
    tform4 t{scale(2, 3, 4)};
    vec3 v2 = t * v1;
    EXPECT_NEAR(v2.x, -8, 1e-6);
    EXPECT_NEAR(v2.y, 18, 1e-6);
    EXPECT_NEAR(v2.z, 32, 1e-6);
}

TEST(TformTest, Rotating1) {
    pnt3 p1{0, 1, 0};
    tform4 t{rotation<Axis::X>(pi_v<float> / 4)};
    pnt3 p2 = t * p1;
    EXPECT_NEAR(p2.x, 0, 1e-6);
    EXPECT_NEAR(p2.y, sqrt2_v<float> / 2, 1e-6);
    EXPECT_NEAR(p2.z, sqrt2_v<float> / 2, 1e-6);
}

TEST(TformTest, Rotating2) {
    pnt3 p1{0, 1, 0};
    tform4 t{inverse(rotation<Axis::X>(pi_v<float> / 4))};
    pnt3 p2 = t * p1;
    EXPECT_NEAR(p2.x, 0, 1e-6);
    EXPECT_NEAR(p2.y, sqrt2_v<float> / 2, 1e-6);
    EXPECT_NEAR(p2.z, sqrt2_v<float> / 2, 1e-6);
}

TEST(TformTest, Shearing) {
    pnt3 p1{2, 3, 4};
    tform4 t{skew(pi_v<float> / 4, {1, 0, 0}, {0, 1, 0})};
    pnt3 p2 = t * p1;
    EXPECT_NEAR(p2.x, 5, 1e-6);
    EXPECT_NEAR(p2.y, 3, 1e-6);
    EXPECT_NEAR(p2.z, 4, 1e-6);
}

TEST(TformTest, Reflecting1) {
    vec3 v1{1, -1, 0};
    vec3 normal{0, 1, 0};
    tform4 t{reflection(normalize(normal))};
    vec3 v2 = t * v1;
    EXPECT_NEAR(v2.x, 1, 1e-6);
    EXPECT_NEAR(v2.y, 1, 1e-6);
    EXPECT_NEAR(v2.z, 0, 1e-6);
}

TEST(TformTest, Reflecting2) {
    vec3 v1{0, -1, 0};
    vec3 normal{sqrt2_v<float> / 2, sqrt2_v<float> / 2, 0};
    tform4 t{reflection(normalize(normal))};
    vec3 v2 = t * v1;
    EXPECT_NEAR(v2.x, 1, 1e-6);
    EXPECT_NEAR(v2.y, 0, 1e-6);
    EXPECT_NEAR(v2.z, 0, 1e-6);
}
