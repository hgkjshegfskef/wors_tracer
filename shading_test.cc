#include "plane.hh"
#include "ray.hh"
#include "shading.hh"
#include "shape.hh"
#include "sphere.hh"

#include <gtest/gtest.h>

#include <numbers>

using namespace wt;
using namespace std::numbers;

TEST(ShadingTest, OutsideIntersection) {
    ray r{pnt3{0, 0, -5}, vec3{0, 0, 1}};
    shape s{sphere{}};
    intersection isec{0, 4};
    shading sh{isec, r, s};

    EXPECT_FALSE(sh.inside);
}

TEST(ShadingTest, InsideIntersection) {
    ray r{pnt3{0, 0, 0}, vec3{0, 0, 1}};
    shape s{sphere{}};
    intersection isec{0, 1};
    shading sh{isec, r, s};

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
    //    shape s{sphere{tform4::translate({0, 0, 1})}};
    shape s = sphere{tform4::translate({0, 0, 1})};
    intersection isec{0, 5};
    shading sh{isec, r, s};
    EXPECT_TRUE(sh.over_pnt.z < -1e-6f / 2);
    EXPECT_TRUE(sh.isec_pnt.z > sh.over_pnt.z);
}

TEST(ShadingTest, ReflectionVector) {
    shape s{plane{}};
    ray r{{0, 1, -1}, {0, -sqrt2_v<float> / .2f, sqrt2_v<float> / .2f}};
    intersection isec{0, sqrt2_v<float>};

    shading sh{isec, r, s};

    EXPECT_EQ(sh.reflect, (vec3{0, sqrt2_v<float> / .2f, sqrt2_v<float> / .2f}));
}
