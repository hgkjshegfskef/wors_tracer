#include "cube.hh"
#include "intersection.hh"
#include "mat3.hh"
#include "ray.hh"
#include "shape.hh"
#include "tform4.hh"

#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

#include <algorithm> // sort

using namespace wt;

struct CubeTest {
    shape s{cube{}};
};

struct CubeIntersectionTest : CubeTest {
    std::vector<intersection> world_isecs;
};

struct CubeHitIntersectionTestParams {
    pnt3 origin;
    vec3 direction;
    float t1;
    float t2;
};

struct CubeHitIntersectionTest : CubeIntersectionTest,
                                 testing::TestWithParam<CubeHitIntersectionTestParams> {};

TEST_P(CubeHitIntersectionTest, Hit) {
    ray r{GetParam().origin, normalize(GetParam().direction)};

    intersect(s, r, 0, world_isecs);
    std::sort(world_isecs.begin(), world_isecs.end());

    ASSERT_EQ(world_isecs.size(), 2);
    EXPECT_EQ(world_isecs[0].t, GetParam().t1);
    EXPECT_EQ(world_isecs[1].t, GetParam().t2);
}

INSTANTIATE_TEST_SUITE_P(
    foobar, CubeHitIntersectionTest,
    testing::Values(CubeHitIntersectionTestParams{{5, .5, 0}, {-1, 0, 0}, 4, 6}, // +x
                    CubeHitIntersectionTestParams{{-5, .5, 0}, {1, 0, 0}, 4, 6}, // -x
                    CubeHitIntersectionTestParams{{.5, 5, 0}, {0, -1, 0}, 4, 6}, // +y
                    CubeHitIntersectionTestParams{{.5, -5, 0}, {0, 1, 0}, 4, 6}, // -y
                    CubeHitIntersectionTestParams{{.5, 0, 5}, {0, 0, -1}, 4, 6}, // +z
                    CubeHitIntersectionTestParams{{.5, 0, -5}, {0, 0, 1}, 4, 6}, // -z
                    CubeHitIntersectionTestParams{{0, .5, 0}, {0, 0, 1}, -1, 1}  // inside
                    ));

struct CubeMissIntersectionTestParams {
    pnt3 origin;
    vec3 direction;
};

struct CubeMissIntersectionTest : CubeIntersectionTest,
                                  testing::TestWithParam<CubeMissIntersectionTestParams> {};

TEST_P(CubeMissIntersectionTest, Miss) {
    ray r{GetParam().origin, normalize(GetParam().direction)};

    intersect(s, r, 0, world_isecs);
    std::sort(world_isecs.begin(), world_isecs.end());

    EXPECT_EQ(world_isecs.size(), 0);
}

INSTANTIATE_TEST_SUITE_P(
    foobar, CubeMissIntersectionTest,
    testing::Values(CubeMissIntersectionTestParams{{-2, 0, 0}, {.2673, .5345, .8018}},
                    CubeMissIntersectionTestParams{{0, -2, 0}, {.8018, .2673, .5345}},
                    CubeMissIntersectionTestParams{{0, 0, -2}, {.5345, .8018, .2673}},
                    CubeMissIntersectionTestParams{{2, 0, 2}, {0, 0, -1}},
                    CubeMissIntersectionTestParams{{0, 2, 2}, {0, -1, 0}},
                    CubeMissIntersectionTestParams{{2, 2, 0}, {-1, 0, 0}}));

struct CubeNormalTestParams {
    pnt3 point;
    vec3 normal;
};

struct CubeNormalTest : CubeTest, testing::TestWithParam<CubeNormalTestParams> {};

TEST_P(CubeNormalTest, Normal) {
    vec3 normal = normal_at(s, GetParam().point, inv_tform(s));

    EXPECT_NEAR(normal.x, GetParam().normal.x, 1e-5f);
    EXPECT_NEAR(normal.y, GetParam().normal.y, 1e-5f);
    EXPECT_NEAR(normal.z, GetParam().normal.z, 1e-5f);
}

INSTANTIATE_TEST_SUITE_P(foobar, CubeNormalTest,
                         testing::Values(CubeNormalTestParams{{1, .5, -.8}, {1, 0, 0}},
                                         CubeNormalTestParams{{-1, -.2, .9}, {-1, 0, 0}},
                                         CubeNormalTestParams{{-.4, 1, -.1}, {0, 1, 0}},
                                         CubeNormalTestParams{{.3, -1, -.7}, {0, -1, 0}},
                                         CubeNormalTestParams{{-.6, .3, 1}, {0, 0, 1}},
                                         CubeNormalTestParams{{.4, .4, -1}, {0, 0, -1}},
                                         CubeNormalTestParams{{1, 1, 1}, {1, 0, 0}},
                                         CubeNormalTestParams{{-1, -1, -1}, {-1, 0, 0}}));
