#include "ray.hh"
#include "shading.hh"
#include "shape.hh"

#include <gtest/gtest.h>

using namespace wt;

TEST(ShadingTest, OutsideIntersection) {
    ray r{pnt3{0, 0, -5}, vec3{0, 0, 1}};
    shape s;
    intersection isec{&s, 4};
    shading sh{isec, r};

    EXPECT_FALSE(sh.inside);
}

TEST(ShadingTest, InsideIntersection) {
    ray r{pnt3{0, 0, 0}, vec3{0, 0, 1}};
    shape s;
    intersection isec{&s, 1};
    shading sh{isec, r};

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
    shape s{tform4::translate({0, 0, 1})};
    intersection isec{&s, 5};
    shading sh{isec, r};
    EXPECT_TRUE(sh.over_pnt.z < -1e-6f / 2);
    EXPECT_TRUE(sh.isec_pnt.z > sh.over_pnt.z);
}