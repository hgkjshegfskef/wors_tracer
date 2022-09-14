#include "cylinder.hh"
#include "intersection.hh"
#include "mat3.hh"
#include "ray.hh"
#include "shape.hh"
#include "tform4.hh"

#include <fmt/format.h>
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

#include <algorithm> // sort

using namespace wt;

struct CylinderTest {
    shape s{cylinder{}};
};

struct CylinderIntersectionTest : CylinderTest {
    std::vector<intersection> world_isecs;
};

struct CylinderHitIntersectionTestParams {
    pnt3 origin;
    vec3 direction;
    float t1;
    float t2;
};

struct CylinderHitIntersectionTest : CylinderIntersectionTest,
                                     testing::TestWithParam<CylinderHitIntersectionTestParams> {};

TEST_P(CylinderHitIntersectionTest, Hit) {
    ray r{GetParam().origin, normalize(GetParam().direction)};

    intersect(s, r, 0, world_isecs);
    std::sort(world_isecs.begin(), world_isecs.end());

    ASSERT_EQ(world_isecs.size(), 2);
    EXPECT_NEAR(world_isecs[0].t, GetParam().t1, 1e-4f);
    EXPECT_NEAR(world_isecs[1].t, GetParam().t2, 1e-4f);
}

INSTANTIATE_TEST_SUITE_P(
    foobar, CylinderHitIntersectionTest,
    testing::Values(CylinderHitIntersectionTestParams{{1, 0, -5}, {0, 0, 1}, 5, 5},
                    CylinderHitIntersectionTestParams{{0, 0, -5}, {0, 0, 1}, 4, 6},
                    CylinderHitIntersectionTestParams{{.5, 0, -5}, {.1, 1, 1}, 6.80798, 7.08872}));

struct CylinderMissIntersectionTestParams {
    pnt3 origin;
    vec3 direction;
};

struct CylinderMissIntersectionTest : CylinderIntersectionTest,
                                      testing::TestWithParam<CylinderMissIntersectionTestParams> {};

TEST_P(CylinderMissIntersectionTest, Miss) {
    ray r{GetParam().origin, normalize(GetParam().direction)};

    intersect(s, r, 0, world_isecs);
    std::sort(world_isecs.begin(), world_isecs.end());

    EXPECT_EQ(world_isecs.size(), 0);
}

INSTANTIATE_TEST_SUITE_P(foobar, CylinderMissIntersectionTest,
                         testing::Values(CylinderMissIntersectionTestParams{{1, 0, 0}, {0, 1, 0}},
                                         CylinderMissIntersectionTestParams{{0, 0, 0}, {0, 1, 0}},
                                         CylinderMissIntersectionTestParams{{0, 0, -5},
                                                                            {1, 1, 1}}));

struct ConstrainedCylinder {
    shape s{cylinder{1, 2}};
};

struct ConstrainedCylinderIntersection : ConstrainedCylinder {
    std::vector<intersection> world_isecs;
};

struct ConstrainedCylinderIntersectionTestParams {
    pnt3 origin;
    vec3 direction;
    unsigned count;
};

template <>
struct fmt::formatter<ConstrainedCylinderIntersectionTestParams> : fmt::formatter<unsigned> {
    auto format(ConstrainedCylinderIntersectionTestParams const& c, fmt::format_context& ctx)
        -> decltype(ctx.out()) {
        auto&& out = ctx.out();
        fmt::format_to(out, "origin: {}, direction: {}, count: {}", c.origin, c.direction, c.count);
        return out;
    }
};

struct ConstrainedCylinderIntersectionTest
    : ConstrainedCylinderIntersection,
      testing::TestWithParam<ConstrainedCylinderIntersectionTestParams> {};

TEST_P(ConstrainedCylinderIntersectionTest, Constrained) {
    ray r{GetParam().origin, normalize(GetParam().direction)};

    intersect(s, r, 0, world_isecs);
    std::sort(world_isecs.begin(), world_isecs.end());

    EXPECT_EQ(world_isecs.size(), GetParam().count);
}

INSTANTIATE_TEST_SUITE_P(
    foobar, ConstrainedCylinderIntersectionTest,
    testing::Values(ConstrainedCylinderIntersectionTestParams{{0, 1.5, 0}, {.1, 1, 0}, 0},
                    ConstrainedCylinderIntersectionTestParams{{0, 3, -5}, {0, 0, 1}, 0},
                    ConstrainedCylinderIntersectionTestParams{{0, 0, -5}, {0, 0, 1}, 0},
                    ConstrainedCylinderIntersectionTestParams{{0, 2, -5}, {0, 0, 1}, 0},
                    ConstrainedCylinderIntersectionTestParams{{0, 1, -5}, {0, 0, 1}, 0},
                    ConstrainedCylinderIntersectionTestParams{{0, 1.5, -2}, {0, 0, 1}, 2}));

struct CappedCylinder {
    shape s{cylinder{1, 2, true}};
};

struct CappedCylinderIntersection : CappedCylinder {
    std::vector<intersection> world_isecs;
};

struct CappedCylinderIntersectionTest
    : CappedCylinderIntersection,
      testing::TestWithParam<ConstrainedCylinderIntersectionTestParams> {};

TEST_P(CappedCylinderIntersectionTest, Capped) {
    ray r{GetParam().origin, normalize(GetParam().direction)};

    intersect(s, r, 0, world_isecs);
    std::sort(world_isecs.begin(), world_isecs.end());

    EXPECT_EQ(world_isecs.size(), GetParam().count) << fmt::format("Test params: {}", GetParam());
}

INSTANTIATE_TEST_SUITE_P(
    foobar, CappedCylinderIntersectionTest,
    testing::Values(ConstrainedCylinderIntersectionTestParams{{0, 3, 0}, {0, -1, 0}, 2},
                    ConstrainedCylinderIntersectionTestParams{{0, 3, -2}, {0, -1, 2}, 2},
                    ConstrainedCylinderIntersectionTestParams{{0, 4, -2}, {0, -1, 1}, 2}, // !
                    ConstrainedCylinderIntersectionTestParams{{0, 0, -2}, {0, 1, 2}, 2},
                    ConstrainedCylinderIntersectionTestParams{{0, -1, -2}, {0, 1, 1}, 2})); // !

struct CylinderNormalTestParams {
    pnt3 point;
    vec3 normal;
};

struct CylinderNormalTest : CylinderTest, testing::TestWithParam<CylinderNormalTestParams> {};

TEST_P(CylinderNormalTest, Normal) {
    vec3 normal = normal_at(s, GetParam().point, inv_tform(s));

    EXPECT_NEAR(normal.x, GetParam().normal.x, 1e-5f);
    EXPECT_NEAR(normal.y, GetParam().normal.y, 1e-5f);
    EXPECT_NEAR(normal.z, GetParam().normal.z, 1e-5f);
}

INSTANTIATE_TEST_SUITE_P(foobar, CylinderNormalTest,
                         testing::Values(CylinderNormalTestParams{{1, 0, 0}, {1, 0, 0}},
                                         CylinderNormalTestParams{{0, 5, -1}, {0, 0, -1}},
                                         CylinderNormalTestParams{{0, -2, 1}, {0, 0, 1}},
                                         CylinderNormalTestParams{{-1, 1, 0}, {-1, 0, 0}}));

struct CappedCylinderNormalTest : CappedCylinder,
                                  testing::TestWithParam<CylinderNormalTestParams> {};

TEST_P(CappedCylinderNormalTest, CappedNormal) {
    vec3 normal = normal_at(s, GetParam().point, inv_tform(s));

    EXPECT_NEAR(normal.x, GetParam().normal.x, 1e-5f);
    EXPECT_NEAR(normal.y, GetParam().normal.y, 1e-5f);
    EXPECT_NEAR(normal.z, GetParam().normal.z, 1e-5f);
}

INSTANTIATE_TEST_SUITE_P(foobar, CappedCylinderNormalTest,
                         testing::Values(CylinderNormalTestParams{{0, 1, 0}, {0, -1, 0}},
                                         CylinderNormalTestParams{{.5, 1, 0}, {0, -1, 0}},
                                         CylinderNormalTestParams{{0, 1, .5}, {0, -1, 0}},
                                         CylinderNormalTestParams{{0, 2, 0}, {0, 1, 0}},
                                         CylinderNormalTestParams{{.5, 2, 0}, {0, 1, 0}},
                                         CylinderNormalTestParams{{0, 2, .5}, {0, 1, 0}}));
