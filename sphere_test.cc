#include "intersection.hh"
#include "mat3.hh"
#include "ray.hh"
#include "shape.hh"
#include "sphere.hh"
#include "tform4.hh"

#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

#include <algorithm> // sort

using namespace wt;

TEST(RayTest, IntersectSphereMiss) {
    ray r{{0, 0, -5}, {0, 0, 1}};
    shape s = sphere{tform4::translate({5, 0, 0})};
    std::vector<intersection> world_isecs;
    ray object_ray{inv_tform(s) * r.origin, inv_tform(s) * r.direction};

    intersect(s, object_ray, 0, world_isecs);
    std::sort(world_isecs.begin(), world_isecs.end());

    EXPECT_EQ(world_isecs.size(), 0);
}

TEST(RayTest, IntersectSphereScaledHit) {
    ray r{{0, 0, -5}, {0, 0, 1}};
    tform4 tform = scale(2, 2, 2);
    shape s = sphere{tform};
    std::vector<intersection> world_isecs;
    ray object_ray{inv_tform(s) * r.origin, inv_tform(s) * r.direction};

    intersect(s, object_ray, 0, world_isecs);
    std::sort(world_isecs.begin(), world_isecs.end());

    EXPECT_EQ(world_isecs.size(), 2);
    EXPECT_EQ(world_isecs[0].t, 3.f);
    EXPECT_EQ(world_isecs[1].t, 7.f);

    pnt3 pi1 = position(r.origin, r.direction, world_isecs[0].t);
    pnt3 pi2 = position(r.origin, r.direction, world_isecs[1].t);

    EXPECT_EQ(pi1, pnt3(0, 0, -2));
    EXPECT_EQ(pi2, pnt3(0, 0, 2));
}

TEST(RayTest, IntersectSphereDefaultHitBehind) {
    ray r{{0, 0, 5}, {0, 0, 1}};
    shape s = sphere{};
    std::vector<intersection> world_isecs;
    ray object_ray{inv_tform(s) * r.origin, inv_tform(s) * r.direction};

    intersect(s, object_ray, 0, world_isecs);
    std::sort(world_isecs.begin(), world_isecs.end());

    EXPECT_EQ(world_isecs.size(), 2);
    EXPECT_EQ(world_isecs[0].t, -6.f);
    EXPECT_EQ(world_isecs[1].t, -4.f);
}

TEST(RayTest, SphereOneHit) {
    ray r{{0, 2, -5}, {0, 0, 1}};
    shape s = sphere{scale(2, 2, 2)};
    std::vector<intersection> world_isecs;
    ray object_ray{inv_tform(s) * r.origin, inv_tform(s) * r.direction};

    intersect(s, object_ray, 0, world_isecs);
    std::sort(world_isecs.begin(), world_isecs.end());

    EXPECT_EQ(world_isecs.size(), 2);

    pnt3 pi1 = position(r.origin, r.direction, world_isecs[0].t);
    pnt3 pi2 = position(r.origin, r.direction, world_isecs[1].t);

    EXPECT_EQ(pi1, pnt3(0, 2, 0));
    EXPECT_EQ(pi2, pnt3(0, 2, 0));
}

TEST(SphereTest, Glass) {
    shape s{sphere{{}, material::glass()}};

    EXPECT_EQ(mater(s).transparency, 1.f);
    EXPECT_EQ(mater(s).refraction_index, 1.5f);
}
