#include "pnt3.hh"

#include <gtest/gtest.h>

#include <cmath>

using namespace wt;

TEST(pnt3test, DefaultCtor) { [[maybe_unused]] pnt3 v; }

TEST(pnt3test, UserDefinedCtor) { [[maybe_unused]] pnt3 v(1.f, 2.f, 3.f); }

TEST(pnt3test, AddVec) {
    pnt3 p{1.f, 1.f, 1.f};
    vec3 v{1.f, 1.f, 1.f};

    pnt3 result1 = p + v;
    pnt3 result2 = v + p;

    pnt3 expected{2.f, 2.f, 2.f};

    for (int i = 0; i < 3; ++i) {
        EXPECT_NEAR(expected[i], result1[i], 1e-6);
        EXPECT_NEAR(expected[i], result2[i], 1e-6);
    }
}

TEST(pnt3test, SubPnt) {
    pnt3 p1{1.f, 1.f, 1.f};
    pnt3 p2{1.f, 1.f, 1.f};

    vec3 result = p1 - p2;

    vec3 expected{0.f, 0.f, 0.f};

    for (int i = 0; i < 3; ++i) {
        EXPECT_NEAR(expected[i], result[i], 1e-6);
    }
}
