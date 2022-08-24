#include "lighting.hh"
#include "material.hh"
#include "pnt_light.hh"
#include "shape.hh"
#include "sphere.hh"
#include "vec3.hh"

#include <gtest/gtest.h>

using namespace wt;

TEST(LightingTest, SurfaceInShadow) {
    material mat;
    shape s{sphere{}};
    pnt3 pos{0, 0, 0};
    vec3 eye{0, 0, -1};
    vec3 normal{0, 0, -1};
    pnt_light light{{0, 0, -10}, {1, 1, 1}};
    bool in_shadow = true;

    color col = lighting(mat, s, light, pos, eye, normal, in_shadow);
    EXPECT_EQ(col.r, 0.1f);
    EXPECT_EQ(col.g, 0.1f);
    EXPECT_EQ(col.b, 0.1f);
}

TEST(LightingTest, WithPattern) {
    material mat{.pattern = stripe_pattern{{1, 1, 1}, {0, 0, 0}, {}, {}},
                 .ambient = 1,
                 .diffuse = 0,
                 .specular = 0};
    shape s{sphere{}};
    vec3 eye{0, 0, -1};
    vec3 normal{0, 0, -1};
    pnt_light light{{0, 0, -10}, {1, 1, 1}};
    bool in_shadow = false;

    color col1 = lighting(mat, s, light, {0.9, 0, 0}, eye, normal, in_shadow);
    color col2 = lighting(mat, s, light, {1.1, 0, 0}, eye, normal, in_shadow);

    EXPECT_EQ(col1.r, 1.f);
    EXPECT_EQ(col1.g, 1.f);
    EXPECT_EQ(col1.b, 1.f);

    EXPECT_EQ(col2.r, 0.f);
    EXPECT_EQ(col2.g, 0.f);
    EXPECT_EQ(col2.b, 0.f);
}
