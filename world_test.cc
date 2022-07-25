#include "intersection.hh"
#include "ray.hh"
#include "world.hh"

#include <gtest/gtest.h>

#include <algorithm> // sort

using namespace wt;

TEST(WorldTest, Default) {
    world w = world::make_default();
    ray world_r{pnt3{0, 0, -5}, vec3{0, 0, 1}};
    auto isecs = intersect(world_r, w);
    std::sort(isecs.begin(), isecs.end());
    EXPECT_EQ(isecs.size(), 4);
    EXPECT_FALSE(isecs[0].empty());
    EXPECT_FALSE(isecs[1].empty());
    EXPECT_FALSE(isecs[2].empty());
    EXPECT_FALSE(isecs[3].empty());
    EXPECT_NEAR(isecs[0].t, 4.f, 1e-6);
    EXPECT_NEAR(isecs[1].t, 4.5f, 1e-6);
    EXPECT_NEAR(isecs[2].t, 5.5f, 1e-6);
    EXPECT_NEAR(isecs[3].t, 6.f, 1e-6);
}
