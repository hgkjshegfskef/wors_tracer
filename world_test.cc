#include "intersection.hh"
#include "material.hh"
#include "plane.hh"
#include "ray.hh"
#include "shading.hh"
#include "shape.hh"
#include "sphere.hh"
#include "test_pattern.hh"
#include "world.hh"

#include <gtest/gtest.h>

#include <algorithm> // sort
#include <cstdlib>   // exit
#include <numbers>

using namespace wt;
using namespace std::numbers;

struct WorldTest : public testing::Test {
    unsigned recursion_depth = 5;
};

TEST_F(WorldTest, Default) {
    world w = world::make_default();
    ray world_r{pnt3{0, 0, -5}, vec3{0, 0, 1}};
    std::vector<intersection> world_isecs;

    intersect(w, world_r, world_isecs);
    std::sort(world_isecs.begin(), world_isecs.end());

    EXPECT_EQ(world_isecs.size(), 4);
    EXPECT_NEAR(world_isecs[0].t, 4.f, 1e-6);
    EXPECT_NEAR(world_isecs[1].t, 4.5f, 1e-6);
    EXPECT_NEAR(world_isecs[2].t, 5.5f, 1e-6);
    EXPECT_NEAR(world_isecs[3].t, 6.f, 1e-6);
}

TEST_F(WorldTest, ShadingIntersection) {
    world w = world::make_default();
    ray r{pnt3{0, 0, -5}, vec3{0, 0, 1}};
    std::vector<intersection> world_isecs{{0, 4}};

    shading sh{w.shapes, r, world_isecs, world_isecs[0]};
    color c = shade_hit(w, sh, world_isecs, recursion_depth);

    EXPECT_NEAR(c.r, .38066f, 1e-5f);
    EXPECT_NEAR(c.g, .47583f, 1e-5f);
    EXPECT_NEAR(c.b, .2855f, 1e-5f);
}

TEST_F(WorldTest, ShadingIntersectionFromInside) {
    world w = world::make_default();
    w.light = pnt_light{pnt3{0.f, .25f, 0.f}, color{1.f, 1.f, 1.f}};
    ray r{pnt3{0, 0, 0}, vec3{0, 0, 1}};
    std::vector<intersection> world_isecs{{1, .5f}};

    shading sh{w.shapes, r, world_isecs, world_isecs[0]};
    color c = shade_hit(w, sh, world_isecs, recursion_depth);

    EXPECT_NEAR(c.r, .90498f, 1e-5f);
    EXPECT_NEAR(c.g, .90498f, 1e-5f);
    EXPECT_NEAR(c.b, .90498f, 1e-5f);
}

TEST_F(WorldTest, ColorAtRayMiss) {
    world w = world::make_default();
    ray r{pnt3{0, 0, -5}, vec3{0, 1, 0}};
    std::vector<intersection> world_isecs;

    color c = color_at(w, r, world_isecs, recursion_depth);

    EXPECT_EQ(c.r, 0);
    EXPECT_EQ(c.g, 0);
    EXPECT_EQ(c.b, 0);
}

TEST_F(WorldTest, ColorAtRayHit) {
    world w = world::make_default();
    ray r{pnt3{0, 0, -5}, vec3{0, 0, 1}};
    std::vector<intersection> world_isecs;

    color c = color_at(w, r, world_isecs, recursion_depth);

    EXPECT_NEAR(c.r, .38066f, 1e-5);
    EXPECT_NEAR(c.g, .47583f, 1e-5);
    EXPECT_NEAR(c.b, .2855f, 1e-5);
}

TEST_F(WorldTest, ColorAtBetweenSpheres) {
    world w = world::make_default();
    shape& outer = w.shapes[0];
    shape& inner = w.shapes[1];
    mater(outer) = {.ambient = 1.f};
    mater(inner) = {.ambient = 1.f};
    // Ray is inside the outer shape, but outside the inner shape, hitting the inner shape.
    ray r{pnt3{0, 0, .75f}, vec3{0, 0, -1}};
    std::vector<intersection> world_isecs;

    color c = color_at(w, r, world_isecs, recursion_depth);

    EXPECT_EQ(c.r, mater(inner).col.r);
    EXPECT_EQ(c.g, mater(inner).col.g);
    EXPECT_EQ(c.b, mater(inner).col.b);
}

TEST_F(WorldTest, PointLightShadow1) {
    world w = world::make_default();
    pnt3 p{0, 10, 0};
    std::vector<intersection> world_isecs;

    EXPECT_FALSE(is_shadowed(w, p, world_isecs));
}

TEST_F(WorldTest, PointLightShadow2) {
    world w = world::make_default();
    pnt3 p{10, -10, 10};
    std::vector<intersection> world_isecs;

    EXPECT_TRUE(is_shadowed(w, p, world_isecs));
}

TEST_F(WorldTest, PointLightShadow3) {
    world w = world::make_default();
    pnt3 p{-20, 20, -20};
    std::vector<intersection> world_isecs;

    EXPECT_FALSE(is_shadowed(w, p, world_isecs));
}

TEST_F(WorldTest, PointLightShadow4) {
    world w = world::make_default();
    pnt3 p{-2, 2, -2};
    std::vector<intersection> world_isecs;

    EXPECT_FALSE(is_shadowed(w, p, world_isecs));
}

TEST_F(WorldTest, IsecInShadow) {
    world w{std::vector<shape>{sphere{}, sphere{tform4::translate({0, 0, 10})}},
            pnt_light{pnt3{0, 0, -10}, color{1, 1, 1}}};
    ray r{pnt3{0, 0, 5}, vec3{0, 0, 1}};
    std::vector<intersection> world_isecs{{1, 4}};

    shading sh{w.shapes, r, world_isecs, world_isecs[0]};
    color c = shade_hit(w, sh, world_isecs, recursion_depth);

    EXPECT_EQ(c.r, .1f);
    EXPECT_EQ(c.g, .1f);
    EXPECT_EQ(c.b, .1f);
}

TEST_F(WorldTest, ReflectedColorForNonreflectiveMaterial) {
    world w = world::make_default();
    ray r{{0, 0, 0}, {0, 0, 1}};
    shape& s = w.shapes[1];
    mater(s).ambient = 1;
    std::vector<intersection> world_isecs{{1, 1}};

    shading sh{w.shapes, r, world_isecs, world_isecs[0]};
    color c = reflected_color(w, sh, world_isecs);

    EXPECT_EQ(c, (color{0, 0, 0}));
}

TEST_F(WorldTest, ReflectedColorForReflectiveMaterial) {
    world w = world::make_default();
    w.shapes.emplace_back(plane{tform4::translate({0, -1, 0}), material{.reflective = .5}});
    ray r{{0, 0, -3}, {0, -sqrt2_v<float> / 2.f, sqrt2_v<float> / 2.f}};
    std::vector<intersection> world_isecs{{2, sqrt2_v<float>}};

    shading sh{w.shapes, r, world_isecs, world_isecs[0]};
    color c = reflected_color(w, sh, world_isecs);

    EXPECT_NEAR(c.r, .19032f, 1e-3f);
    EXPECT_NEAR(c.g, .2379f, 1e-3f);
    EXPECT_NEAR(c.b, .14274f, 1e-3f);
}

TEST_F(WorldTest, ShadeHitWithReflectiveMaterial) {
    world w = world::make_default();
    w.shapes.emplace_back(plane{tform4::translate({0, -1, 0}), material{.reflective = .5}});
    ray r{{0, 0, -3}, {0, -sqrt2_v<float> / 2.f, sqrt2_v<float> / 2.f}};
    std::vector<intersection> world_isecs{{2, sqrt2_v<float>}};

    shading sh{w.shapes, r, world_isecs, world_isecs[0]};
    color c = shade_hit(w, sh, world_isecs, recursion_depth);

    EXPECT_NEAR(c.r, .87677f, 1e-3f);
    EXPECT_NEAR(c.g, .92436f, 1e-3f);
    EXPECT_NEAR(c.b, .82918f, 1e-3f);
}

TEST(WorldDeathTest, MutuallyReflectiveSurfaces) {
    world w{{plane{tform4::translate({0, -1, 0}), material{.reflective = 1}},
             plane{tform4::translate({0, 1, 0}), material{.reflective = 1}}},
            pnt_light{pnt3{0, 0, 0}, color{1, 1, 1}}};
    ray r{pnt3{0, 0, 0}, vec3{0, 1, 0}};
    std::vector<intersection> world_isecs;

    GTEST_FLAG_SET(death_test_style, "threadsafe");
    EXPECT_EXIT(
        {
            color_at(w, r, world_isecs, 10);
            std::exit(0);
        },
        testing::ExitedWithCode(0), "");
}

TEST_F(WorldTest, RefractedColorOfOpaqueSurface) {
    world w = world::make_default();
    ray r{pnt3{0, 0, -5}, vec3{0, 0, 1}};
    std::vector<intersection> world_isecs{{0, 4}, {0, 6}};

    shading sh{w.shapes, r, world_isecs, world_isecs[0]};
    color c = refracted_color(w, sh, world_isecs, recursion_depth);

    EXPECT_EQ(c, (color{0, 0, 0}));
}

TEST_F(WorldTest, RefractedColorAtMaxDepth) {
    world w = world::make_default();
    mater(w.shapes[0]) = material::glass();
    ray r{pnt3{0, 0, -5}, vec3{0, 0, 1}};
    std::vector<intersection> world_isecs{{0, 4}, {0, 6}};

    shading sh{w.shapes, r, world_isecs, world_isecs[0]};
    color c = refracted_color(w, sh, world_isecs, 0);

    EXPECT_EQ(c, (color{0, 0, 0}));
}

TEST_F(WorldTest, RefractedColorTotalInternalReflection) {
    world w = world::make_default();
    mater(w.shapes[0]) = material::glass();
    ray r{pnt3{0, 0, sqrt2_v<float> / 2}, vec3{0, 1, 0}};
    std::vector<intersection> world_isecs{{0, -sqrt2_v<float> / 2}, {0, sqrt2_v<float> / 2}};

    shading sh{w.shapes, r, world_isecs, world_isecs[1]}; // inside the sphere
    color c = refracted_color(w, sh, world_isecs, recursion_depth);

    EXPECT_EQ(c, (color{0, 0, 0}));
}

TEST_F(WorldTest, RefractedColorOfRefractedRay) {
    world w = world::make_default();
    mater(w.shapes[0]) = material{.pattern = test_pattern{}, .ambient = 1};
    mater(w.shapes[1]) = material::glass();
    ray r{pnt3{0, 0, .1}, vec3{0, 1, 0}};
    std::vector<intersection> world_isecs{{0, -.9899}, {1, -.4899}, {1, .4899}, {0, .9899}};

    shading sh{w.shapes, r, world_isecs, world_isecs[2]};
    color c = refracted_color(w, sh, world_isecs, recursion_depth);

    EXPECT_NEAR(c.r, 0.f, 1e-6f);
    EXPECT_NEAR(c.g, .99888f, 1e-5f);
    EXPECT_NEAR(c.b, .04725f, 1e-3f);
}

TEST_F(WorldTest, ShadeHitWithTransparentMaterial) {
    world w = world::make_default();
    plane floor{tform4::translate({0, -1, 0}), material{.transparency = .5, .refractive = 1.5}};
    w.shapes.emplace_back(floor);
    sphere ball{tform4::translate({0, -3.5, -.5}), material{.col = {1, 0, 0}, .ambient = .5}};
    w.shapes.emplace_back(ball);
    ray r{pnt3{0, 0, -3}, vec3{0, -sqrt2_v<float> / 2.f, sqrt2_v<float> / 2.f}};
    std::vector<intersection> world_isecs{{2 /*floor*/, sqrt2_v<float>}};

    shading sh{w.shapes, r, world_isecs, world_isecs[0]};
    color c = shade_hit(w, sh, world_isecs, recursion_depth);

    EXPECT_NEAR(c.r, .93642f, 1e-5f);
    EXPECT_NEAR(c.g, .68642f, 1e-5f);
    EXPECT_NEAR(c.b, .68642f, 1e-5f);
}

TEST_F(WorldTest, SchlickTotalInternalReflection) {
    std::vector<shape> shapes{{sphere{{}, material::glass()}}};
    ray r{pnt3{0, 0, sqrt2_v<float> / 2}, vec3{0, 1, 0}};
    std::vector<intersection> world_isecs{{0, -sqrt2_v<float> / 2}, {0, sqrt2_v<float> / 2}};

    shading sh{shapes, r, world_isecs, world_isecs[1]};
    float reflectance = schlick(sh);

    EXPECT_EQ(reflectance, 1.f);
}

TEST_F(WorldTest, SchlickPerpendicularAngle) {
    std::vector<shape> shapes{{sphere{{}, material::glass()}}};
    ray r{pnt3{0, 0, 0}, vec3{0, 1, 0}};
    std::vector<intersection> world_isecs{{0, -1}, {0, 1}};

    shading sh{shapes, r, world_isecs, world_isecs[1]};
    float reflectance = schlick(sh);

    EXPECT_NEAR(reflectance, .04f, 1e-5f);
}

TEST_F(WorldTest, SchlickSmallAngle) {
    std::vector<shape> shapes{{sphere{{}, material::glass()}}};
    ray r{pnt3{0, .99, -2}, vec3{0, 0, 1}};
    std::vector<intersection> world_isecs{{0, 1.8589}};

    shading sh{shapes, r, world_isecs, world_isecs[0]};
    float reflectance = schlick(sh);

    EXPECT_NEAR(reflectance, .48873f, 1e-5f);
}

TEST_F(WorldTest, ShadeHitWithTransparentReflectiveMaterial) {
    world w = world::make_default();
    plane floor{tform4::translate({0, -1, 0}),
                material{.reflective = .5, .transparency = .5, .refractive = 1.5}};
    w.shapes.emplace_back(floor);
    sphere ball{tform4::translate({0, -3.5, -.5}), material{.col = {1, 0, 0}, .ambient = .5}};
    w.shapes.emplace_back(ball);
    ray r{pnt3{0, 0, -3}, vec3{0, -sqrt2_v<float> / 2.f, sqrt2_v<float> / 2.f}};
    std::vector<intersection> world_isecs{{2 /*floor*/, sqrt2_v<float>}};

    shading sh{w.shapes, r, world_isecs, world_isecs[0]};
    color c = shade_hit(w, sh, world_isecs, recursion_depth);

    EXPECT_NEAR(c.r, .93391f, 1e-4f);
    EXPECT_NEAR(c.g, .69643f, 1e-4f);
    EXPECT_NEAR(c.b, .69243f, 1e-4f);
}
