#include "intersection.hh"
#include "ray.hh"
#include "shading.hh"
#include "shape.hh"
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

TEST(WorldTest, ShadingIntersection) {
    world w = world::make_default();
    ray r{pnt3{0, 0, -5}, vec3{0, 0, 1}};
    shape const& s = w.shapes[0];
    intersection isec{&s, 4};
    shading sh{isec, r};

    color c = shade_hit(w, sh);
    EXPECT_NEAR(c.r, .38066f, 1e-5f);
    EXPECT_NEAR(c.g, .47583f, 1e-5f);
    EXPECT_NEAR(c.b, .2855f, 1e-5f);
}

TEST(WorldTest, ShadingIntersectionFromInside) {
    world w = world::make_default();
    w.light = pnt_light{pnt3{0.f, .25f, 0.f}, color{1.f, 1.f, 1.f}};
    ray r{pnt3{0, 0, 0}, vec3{0, 0, 1}};
    shape const& s = w.shapes[1];
    intersection isec{&s, .5f};
    shading sh{isec, r};

    color c = shade_hit(w, sh);
    EXPECT_NEAR(c.r, .90498f, 1e-5f);
    EXPECT_NEAR(c.g, .90498f, 1e-5f);
    EXPECT_NEAR(c.b, .90498f, 1e-5f);
}

TEST(WorldTest, ColorAtRayMiss) {
    world w = world::make_default();
    ray r{pnt3{0, 0, -5}, vec3{0, 1, 0}};
    color c = color_at(w, r);
    EXPECT_EQ(c.r, 0);
    EXPECT_EQ(c.g, 0);
    EXPECT_EQ(c.b, 0);
}

TEST(WorldTest, ColorAtRayHit) {
    world w = world::make_default();
    ray r{pnt3{0, 0, -5}, vec3{0, 0, 1}};
    color c = color_at(w, r);
    EXPECT_NEAR(c.r, .38066f, 1e-5);
    EXPECT_NEAR(c.g, .47583f, 1e-5);
    EXPECT_NEAR(c.b, .2855f, 1e-5);
}

TEST(WorldTest, ColorAtBetweenSpheres) {
    world w = world::make_default();
    shape& outer = w.shapes[0];
    shape& inner = w.shapes[1];
    outer.mat.ambient = 1.f;
    inner.mat.ambient = 1.f;
    // Ray is inside the outer shape, but outside the inner shape, hitting the inner shape.
    ray r{pnt3{0, 0, .75f}, vec3{0, 0, -1}};

    color c = color_at(w, r);
    EXPECT_EQ(c.r, inner.mat.col.r);
    EXPECT_EQ(c.g, inner.mat.col.g);
    EXPECT_EQ(c.b, inner.mat.col.b);
}

TEST(WorldTest, PointLightShadow1) {
    world w = world::make_default();
    pnt3 p{0, 10, 0};
    EXPECT_FALSE(is_shadowed(w, p));
}

TEST(WorldTest, PointLightShadow2) {
    world w = world::make_default();
    pnt3 p{10, -10, 10};
    EXPECT_TRUE(is_shadowed(w, p));
}

TEST(WorldTest, PointLightShadow3) {
    world w = world::make_default();
    pnt3 p{-20, 20, -20};
    EXPECT_FALSE(is_shadowed(w, p));
}

TEST(WorldTest, PointLightShadow4) {
    world w = world::make_default();
    pnt3 p{-2, 2, -2};
    EXPECT_FALSE(is_shadowed(w, p));
}

TEST(WorldTest, IsecInShadow) {
    world w{pnt_light{pnt3{0, 0, -10}, color{1, 1, 1}},
            std::vector{shape{}, shape{tform4::translate({0, 0, 10})}}};
    ray r{pnt3{0, 0, 5}, vec3{0, 0, 1}};
    intersection isec{&w.shapes[1], 4};
    shading sh{isec, r};
    color c = shade_hit(w, sh);
    EXPECT_EQ(c.r, .1f);
    EXPECT_EQ(c.g, .1f);
    EXPECT_EQ(c.b, .1f);
}
