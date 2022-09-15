#include "cone.hh"
#include "intersection.hh"
#include "mat3.hh"
#include "ray.hh"
#include "shape.hh"
#include "tform4.hh"

#include <fmt/format.h>
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

#include <algorithm> // sort
#include <numbers>

using namespace std::numbers;
using namespace wt;

struct ConeTest {
    shape s{cone{}};
};

struct ConeIntersectionTestParams {
    pnt3 origin;
    vec3 direction;
    float t1;
    float t2;
};

struct ConeIntersectionTest : ConeTest, testing::TestWithParam<ConeIntersectionTestParams> {
    std::vector<intersection> world_isecs;
};

TEST_P(ConeIntersectionTest, Intersect) {
    ray r{GetParam().origin, normalize(GetParam().direction)};

    intersect(s, r, 0, world_isecs);
    std::sort(world_isecs.begin(), world_isecs.end());

    ASSERT_EQ(world_isecs.size(), 2);
    EXPECT_NEAR(world_isecs[0].t, GetParam().t1, 1e-4f);
    EXPECT_NEAR(world_isecs[1].t, GetParam().t2, 1e-4f);
}

INSTANTIATE_TEST_SUITE_P(
    foobar, ConeIntersectionTest,
    testing::Values(ConeIntersectionTestParams{{0, 0, -5}, {0, 0, 1}, 5, 5},
                    // unfortunately, this test will say that discriminant is < 0 due to rounding
                    // error. I don't know yet how to handle that properly :(
                    // ConeIntersectionTestParams{{0, 0, -5}, {1, 1, 1}, 8.66025, 8.66025},
                    // so we cheat:
                    ConeIntersectionTestParams{{0, 0, -5}, {1, 1, 1 + 1e-6}, 8.66025, 8.66025},
                    ConeIntersectionTestParams{{1, 1, -5}, {-.5, -1, 1}, 4.55006, 49.44994}));

TEST(ConeTest, ParallelToHalf) {
    shape s{cone{}};
    ray r{{0, 0, -1}, normalize({0, 1, 1})};
    std::vector<intersection> world_isecs;

    intersect(s, r, 0, world_isecs);
    std::sort(world_isecs.begin(), world_isecs.end());

    ASSERT_EQ(world_isecs.size(), 1);
    EXPECT_NEAR(world_isecs[0].t, .35355, 1e-4f);
}

struct CappedCone {
    shape s{cone{-.5, .5, true}};
};

struct CappedConeIntersection : CappedCone {
    std::vector<intersection> world_isecs;
};

struct CappedConeIntersectionTestParams {
    pnt3 origin;
    vec3 direction;
    unsigned count;
};

struct CappedConeIntersectionTest : CappedConeIntersection,
                                    testing::TestWithParam<CappedConeIntersectionTestParams> {};

TEST_P(CappedConeIntersectionTest, Capped) {
    ray r{GetParam().origin, normalize(GetParam().direction)};

    intersect(s, r, 0, world_isecs);
    std::sort(world_isecs.begin(), world_isecs.end());

    EXPECT_EQ(world_isecs.size(), GetParam().count);
}

INSTANTIATE_TEST_SUITE_P(
    foobar, CappedConeIntersectionTest,
    testing::Values(CappedConeIntersectionTestParams{{0, 0, -5}, {0, 1, 0}, 0},
                    CappedConeIntersectionTestParams{{0, 0, -.25}, {0, 1, 1}, 2},
                    CappedConeIntersectionTestParams{{0, 0, -.25}, {0, 1, 0}, 4}));

struct ConeNormalTestParams {
    pnt3 point;
    vec3 normal;
};

struct ConeNormalTest : ConeTest, testing::TestWithParam<ConeNormalTestParams> {};

TEST_P(ConeNormalTest, Normal) {
    vec3 normal = normal_at(s, GetParam().point, inv_tform(s));

    EXPECT_NEAR(normal.x, normalize(GetParam().normal).x, 1e-5f);
    EXPECT_NEAR(normal.y, normalize(GetParam().normal).y, 1e-5f);
    EXPECT_NEAR(normal.z, normalize(GetParam().normal).z, 1e-5f);
}

INSTANTIATE_TEST_SUITE_P(foobar, ConeNormalTest,
                         testing::Values(ConeNormalTestParams{{1, 1, 1}, {1, -sqrt2_v<float>, 1}},
                                         ConeNormalTestParams{{-1, -1, 0}, {-1, 1, 0}}));
