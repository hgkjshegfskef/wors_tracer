#include "sphere.hh"
#include "tform4.hh"

#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

#include <numbers>

using namespace wt;
using namespace std::numbers;

TEST(NormalTest, SphereNoTform) {
    sphere s;
    pnt3 const s_center{0.f, 0.f, 0.f};
    pnt3 const s_point{1.f, 0.f, 0.f};

    vec3 normal = normalize(s_point - s_center);
    EXPECT_NEAR(normal.x, 1.f, 1e-6);
    EXPECT_NEAR(normal.y, 0.f, 1e-6);
    EXPECT_NEAR(normal.z, 0.f, 1e-6);
}

TEST(NormalTest, SphereTranslated) {
    sphere s;
    s.tform.set_translation({0, 1, 0});
    pnt3 const s_center = s.tform * pnt3{0, 0, 0};
    EXPECT_NEAR(s_center.x, 0, 1e-6);
    EXPECT_NEAR(s_center.y, 1, 1e-6);
    EXPECT_NEAR(s_center.z, 0, 1e-6);
    pnt3 const s_point{0, 1.70711f, -.70711f};

    vec3 normal = normalize(normalize(s_point - s_center) * inverse(s.tform));
    EXPECT_NEAR(normal.x, 0, 1e-6);
    EXPECT_NEAR(normal.y, .70711f, 1e-5);
    EXPECT_NEAR(normal.z, -.70711f, 1e-5);
}

TEST(NormalTest, SphereScaledRotated) {
    sphere s{scale(1.f, .5f, 1.f) * rotation<Axis::Z>(pi_v<float> / 5.f)};
    pnt3 const s_center{0.f, 0.f, 0.f};
    pnt3 const s_point{0.f, sqrt2_v<float> / 2.f, -sqrt2_v<float> / 2.f};

    tform4 const inv_tform{inverse(s.tform)};
    pnt3 const object_s_point{inv_tform * s_point};

    vec3 normal = normalize(normalize(object_s_point - s_center) * inv_tform);
    EXPECT_NEAR(normal.x, 0.f, 1e-6);
    EXPECT_NEAR(normal.y, .970142f, 1e-6);
    EXPECT_NEAR(normal.z, -0.242535f, 1e-6);
}
