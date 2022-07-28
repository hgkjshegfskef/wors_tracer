#include "lighting.hh"
#include "material.hh"
#include "pnt_light.hh"
#include "vec3.hh"

#include <gtest/gtest.h>

using namespace wt;

TEST(LightingTest, SurfaceInShadow) {
    material mat;
    pnt3 pos{0, 0, 0};
    vec3 eye{0, 0, -1};
    vec3 normal{0, 0, -1};
    pnt_light light{{0, 0, -10}, {1, 1, 1}};
    bool in_shadow = true;

    color col = lighting(mat, light, pos, eye, normal, in_shadow);
    EXPECT_EQ(col.r, 0.1f);
    EXPECT_EQ(col.g, 0.1f);
    EXPECT_EQ(col.b, 0.1f);
}
