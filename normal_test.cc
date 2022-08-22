#include "mat3.hh"
#include "pnt3.hh"
#include "shape.hh"
#include "sphere.hh"
#include "tform4.hh"

#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

#include <numbers>

using namespace wt;
using namespace std::numbers;

TEST(NormalTest, SphereNoTform) {
    shape s = sphere{};
    pnt3 const s_point{1.f, 0.f, 0.f};

    vec3 normal = normal_at(s_point, tform4{});

    EXPECT_NEAR(normal.x, 1.f, 1e-6);
    EXPECT_NEAR(normal.y, 0.f, 1e-6);
    EXPECT_NEAR(normal.z, 0.f, 1e-6);
}

TEST(NormalTest, SphereTranslated) {
    shape s{sphere{tform4::translate({0, 1, 0})}};
    pnt3 const s_point{0, 1.70711f, -.70711f};

    vec3 normal = normal_at(s_point, inv_tform(s));

    EXPECT_NEAR(normal.x, 0, 1e-6);
    EXPECT_NEAR(normal.y, .70711f, 1e-5);
    EXPECT_NEAR(normal.z, -.70711f, 1e-5);
}

TEST(NormalTest, SphereScaledRotated) {
    shape s{sphere{scale(1.f, .5f, 1.f) * rotation<Axis::Z>(pi_v<float> / 5.f)}};
    pnt3 const s_point{0.f, sqrt2_v<float> / 2.f, -sqrt2_v<float> / 2.f};

    vec3 normal = normal_at(s_point, inv_tform(s));

    EXPECT_NEAR(normal.x, 0.f, 1e-6);
    EXPECT_NEAR(normal.y, .970142f, 1e-6);
    EXPECT_NEAR(normal.z, -0.242535f, 1e-6);
}

TEST(NormalTest, NormalAtNoTform) {
    shape s{sphere{}};
    pnt3 const s_point{1.f, 0.f, 0.f};

    vec3 normal = normal_at(s_point, inv_tform(s));

    EXPECT_NEAR(normal.x, 1.f, 1e-6);
    EXPECT_NEAR(normal.y, 0.f, 1e-6);
    EXPECT_NEAR(normal.z, 0.f, 1e-6);
}

TEST(NormalTest, NormalAtSphereTranslated) {
    shape s{sphere{tform4::translate({0, 1, 0})}};
    pnt3 const s_point{0, 1.70711f, -.70711f};

    vec3 normal = normal_at(s_point, inv_tform(s));

    EXPECT_NEAR(normal.x, 0, 1e-6);
    EXPECT_NEAR(normal.y, .70711f, 1e-5);
    EXPECT_NEAR(normal.z, -.70711f, 1e-5);
}

TEST(NormalTest, NormalAtSphereScaledRotated) {
    shape s{sphere{scale(1.f, .5f, 1.f) * rotation<Axis::Z>(pi_v<float> / 5.f)}};
    pnt3 const s_point{0.f, sqrt2_v<float> / 2.f, -sqrt2_v<float> / 2.f};

    vec3 normal = normal_at(s_point, inv_tform(s));

    EXPECT_NEAR(normal.x, 0.f, 1e-6);
    EXPECT_NEAR(normal.y, .970142f, 1e-6);
    EXPECT_NEAR(normal.z, -0.242535f, 1e-6);
}
