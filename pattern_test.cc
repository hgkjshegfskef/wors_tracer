#include "mat3.hh"
#include "pnt3.hh"
#include "shape.hh"
#include "sphere.hh"
#include "stripe_pattern.hh"

#include <gtest/gtest.h>

using namespace wt;

struct PatternTest : public testing::Test {
    color black{0, 0, 0};
    color white{1, 1, 1};
};

TEST_F(PatternTest, PatternCtor) {
    stripe_pattern pattern{white, black, {}, {}};

    EXPECT_EQ(pattern.first, white);
    EXPECT_EQ(pattern.second, black);
}

TEST_F(PatternTest, XAxis) {
    stripe_pattern pattern{white, black, {}, {}};

    EXPECT_EQ(stripe_at(pattern, pnt3{0, 0, 0}), white);
    EXPECT_EQ(stripe_at(pattern, pnt3{0.9, 0, 0}), white);
    EXPECT_EQ(stripe_at(pattern, pnt3{1, 0, 0}), black);
    EXPECT_EQ(stripe_at(pattern, pnt3{-0.1, 0, 0}), black);
    EXPECT_EQ(stripe_at(pattern, pnt3{-1, 0, 0}), black);
    EXPECT_EQ(stripe_at(pattern, pnt3{-1.1, 0, 0}), white);
}

TEST_F(PatternTest, YAxis) {
    stripe_pattern pattern{white, black, {}, {}};

    EXPECT_EQ(stripe_at(pattern, pnt3{0, 0, 0}), white);
    EXPECT_EQ(stripe_at(pattern, pnt3{0, 1, 0}), white);
    EXPECT_EQ(stripe_at(pattern, pnt3{0, 2, 0}), white);
}

TEST_F(PatternTest, ZAxis) {
    stripe_pattern pattern{white, black, {}, {}};

    EXPECT_EQ(stripe_at(pattern, pnt3{0, 0, 0}), white);
    EXPECT_EQ(stripe_at(pattern, pnt3{0, 0, 1}), white);
    EXPECT_EQ(stripe_at(pattern, pnt3{0, 0, 2}), white);
}

TEST_F(PatternTest, WithShapeTform) {
    shape s{sphere{scale(2, 2, 2)}};
    stripe_pattern pattern{white, black, {}, {}};

    color c = stripe_at(pattern, s, {1.5, 0, 0});

    EXPECT_EQ(c, white);
}

TEST_F(PatternTest, WithPatternTform) {
    shape s{sphere{}};
    stripe_pattern pattern{white, black, {scale(2, 2, 2)}, inverse(pattern.tform)};

    color c = stripe_at(pattern, s, {1.5, 0, 0});

    EXPECT_EQ(c, white);
}

TEST_F(PatternTest, WithShapeAndPatternTform) {
    shape s{sphere{scale(2, 2, 2)}};
    stripe_pattern pattern{white, black, {tform4::translate({.5, 0, 0})}, inverse(pattern.tform)};

    color c = stripe_at(pattern, s, {2.5, 0, 0});

    EXPECT_EQ(c, white);
}
