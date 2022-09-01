#include "intersection.hh"
#include "mat3.hh"
#include "plane.hh"
#include "ray.hh"
#include "shading.hh"
#include "shape.hh"
#include "sphere.hh"

#include <gtest/gtest.h>

#include <numbers>
#include <vector>

using namespace wt;
using namespace std::numbers;

TEST(ShadingTest, OutsideIntersection) {
    ray r{pnt3{0, 0, -5}, vec3{0, 0, 1}};
    std::vector<shape> shapes{sphere{}};
    intersection isec{0, 4};
    std::vector<intersection> world_isecs;

    shading sh{shapes, r, world_isecs, isec};

    EXPECT_FALSE(sh.inside);
}

TEST(ShadingTest, InsideIntersection) {
    ray r{pnt3{0, 0, 0}, vec3{0, 0, 1}};
    std::vector<shape> shapes{sphere{}};
    intersection isec{0, 1};
    std::vector<intersection> world_isecs;

    shading sh{shapes, r, world_isecs, isec};

    EXPECT_EQ(sh.isec_pnt.x, 0);
    EXPECT_EQ(sh.isec_pnt.y, 0);
    EXPECT_EQ(sh.isec_pnt.z, 1);
    EXPECT_EQ(sh.eye.x, 0);
    EXPECT_EQ(sh.eye.y, 0);
    EXPECT_EQ(sh.eye.z, -1);
    EXPECT_TRUE(sh.inside);
    EXPECT_EQ(sh.normal.x, 0);
    EXPECT_EQ(sh.normal.y, 0);
    EXPECT_EQ(sh.normal.z, -1);
}

TEST(ShadingTest, HitOffsetsPoint) {
    ray r{pnt3{0, 0, -5}, vec3{0, 0, 1}};
    std::vector<shape> shapes{sphere{tform4::translate({0, 0, 1})}};
    intersection isec{0, 5};
    std::vector<intersection> world_isecs;

    shading sh{shapes, r, world_isecs, isec};

    EXPECT_TRUE(sh.over_pnt.z < -1e-6f / 2);
    EXPECT_TRUE(sh.isec_pnt.z > sh.over_pnt.z);
}

TEST(ShadingTest, ReflectionVector) {
    std::vector<shape> shapes{plane{}};
    ray r{{0, 1, -1}, {0, -sqrt2_v<float> / .2f, sqrt2_v<float> / .2f}};
    intersection isec{0, sqrt2_v<float>};
    std::vector<intersection> world_isecs;

    shading sh{shapes, r, world_isecs, isec};

    EXPECT_EQ(sh.reflect, (vec3{0, sqrt2_v<float> / .2f, sqrt2_v<float> / .2f}));
}

struct RefractionTestExamples {
    unsigned idx;
    float n1, n2;
};

class RefractionTest : public testing::TestWithParam<RefractionTestExamples> {
  protected:
    void SetUp() override {
        shapes = {
            sphere{scale(2, 2, 2), {.transparency = 1, .refraction_index = 1.5}},
            sphere{tform4::translate({0, 0, -.25}), {.transparency = 1, .refraction_index = 2}},
            sphere{tform4::translate({0, 0, .25}), {.transparency = 1, .refraction_index = 2.5}}};
        r = {pnt3{0, 0, -4}, vec3{0, 0, 1}};
        isecs = {{0, 2}, {1, 2.75}, {2, 3.25}, {1, 4.75}, {2, 5.25}, {0, 6}};
    }

    ray r;
    std::vector<shape> shapes;
    std::vector<intersection> isecs;
};

TEST_P(RefractionTest, n1n2) {
    auto param = GetParam();

    shading sh{shapes, r, isecs, isecs[param.idx]};

    EXPECT_NEAR(param.n1, sh.n1, 1e-6f);
    EXPECT_NEAR(param.n2, sh.n2, 1e-6f);
}

INSTANTIATE_TEST_SUITE_P(
    FooBar, RefractionTest,
    testing::Values(RefractionTestExamples{0, 1.0, 1.5}, RefractionTestExamples{1, 1.5, 2.0},
                    RefractionTestExamples{2, 2.0, 2.5}, RefractionTestExamples{3, 2.5, 2.5},
                    RefractionTestExamples{4, 2.5, 1.5}, RefractionTestExamples{5, 1.5, 1.0}));

TEST(ShadingTest, UnderPoint) {
    ray r{pnt3{0, 0, -5}, vec3{0, 0, 1}};
    std::vector<shape> shapes{sphere{tform4::translate({0, 0, 1}), material::glass()}};
    intersection isec{0, 5};
    std::vector<intersection> world_isecs;

    shading sh{shapes, r, world_isecs, isec};

    EXPECT_GT(sh.under_pnt.z, 1e-5f / 2.f);
    EXPECT_LT(sh.isec_pnt.z, sh.under_pnt.z);
}
