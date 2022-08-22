#include "intersection.hh"
#include "plane.hh"
#include "ray.hh"
#include "shape.hh"

#include <gtest/gtest.h>

#include <algorithm> // sort

using namespace wt;

TEST(PlaneTest, RayParallel) {
    shape p{plane{}};
    ray r{pnt3{0, 10, 0}, vec3{0, 0, 1}};
    ray object_ray{inv_tform(p) * r.origin, inv_tform(p) * r.direction};
    std::vector<intersection> world_isecs;

    intersect(p, object_ray, 0, world_isecs);
    std::sort(world_isecs.begin(), world_isecs.end());

    EXPECT_EQ(world_isecs.size(), 0);
}

TEST(PlaneTest, RayCoplanar) {
    shape p{plane{}};
    ray r{pnt3{0, 0, 0}, vec3{0, 0, 1}};
    ray object_ray{inv_tform(p) * r.origin, inv_tform(p) * r.direction};
    std::vector<intersection> world_isecs;

    intersect(p, object_ray, 0, world_isecs);
    std::sort(world_isecs.begin(), world_isecs.end());

    EXPECT_EQ(world_isecs.size(), 0);
}

TEST(PlaneTest, RayFromAbove) {
    shape p{plane{}};
    ray r{pnt3{0, 1, 0}, vec3{0, -1, 0}};
    ray object_ray{inv_tform(p) * r.origin, inv_tform(p) * r.direction};
    std::vector<intersection> world_isecs;

    intersect(p, object_ray, 0, world_isecs);
    std::sort(world_isecs.begin(), world_isecs.end());

    EXPECT_EQ(world_isecs.size(), 1);
    EXPECT_EQ(world_isecs[0].t, 1);

    pnt3 pos = position(r.origin, r.direction, world_isecs[0].t);

    EXPECT_EQ(pos.x, 0.f);
    EXPECT_EQ(pos.y, 0.f);
    EXPECT_EQ(pos.z, 0.f);
}

TEST(PlaneTest, RayFromBelow) {
    shape p{plane{}};
    ray r{pnt3{0, -1, 0}, vec3{0, 1, 0}};
    ray object_ray{inv_tform(p) * r.origin, inv_tform(p) * r.direction};
    std::vector<intersection> world_isecs;

    intersect(p, object_ray, 0, world_isecs);
    std::sort(world_isecs.begin(), world_isecs.end());

    EXPECT_EQ(world_isecs.size(), 1);
    EXPECT_EQ(world_isecs[0].t, 1);

    pnt3 pos = position(r.origin, r.direction, world_isecs[0].t);

    EXPECT_EQ(pos.x, 0.f);
    EXPECT_EQ(pos.y, 0.f);
    EXPECT_EQ(pos.z, 0.f);
}
