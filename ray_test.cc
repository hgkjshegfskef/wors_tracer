#include "ray.hh"

#include <gtest/gtest.h>

#include <algorithm>

using namespace wt;

TEST(RayTest, v2Intersect1) {
    ray r{{0, 0, -5}, {0, 0, 1}};
    tform4 tform;
    tform.set_translation({5, 0, 0});
    sphere s{tform};

    auto isections = v2::intersect(r, s);
    EXPECT_EQ(isections.size(), 2);
    EXPECT_TRUE(isections[0].empty());
    EXPECT_TRUE(isections[1].empty());
}

TEST(RayTest, v2Intersect2) {
    ray r{{0, 0, -5}, {0, 0, 1}};
    tform4 tform = scale(2, 2, 2);
    sphere s{tform};

    auto isections = v2::intersect(r, s);
    EXPECT_EQ(isections.size(), 2);
    std::sort(isections.begin(), isections.end());
    EXPECT_EQ(isections[0].t, 3.f);
    EXPECT_EQ(isections[1].t, 7.f);

    pnt3 pi1 = position(r.origin, r.direction, isections[0].t);
    pnt3 pi2 = position(r.origin, r.direction, isections[1].t);
    EXPECT_EQ(pi1, pnt3(0, 0, -2));
    EXPECT_EQ(pi2, pnt3(0, 0, 2));
}

TEST(RayTest, v2Intersect3) {
    ray r{{0, 0, 5}, {0, 0, 1}};
    sphere s;

    auto isections = v2::intersect(r, s);
    EXPECT_EQ(isections.size(), 2);
    std::sort(isections.begin(), isections.end());
    EXPECT_EQ(isections[0].t, -6.f);
    EXPECT_EQ(isections[1].t, -4.f);
}
