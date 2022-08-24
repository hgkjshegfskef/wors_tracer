#include "gradient_pattern.hh"
#include "mat3.hh"
#include "pattern.hh"
#include "pnt3.hh"
#include "ring_pattern.hh"
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
    pattern pattern = stripe_pattern{white, black};

    EXPECT_EQ(first(pattern), white);
    EXPECT_EQ(second(pattern), black);
}

TEST_F(PatternTest, XAxis) {
    pattern pattern{stripe_pattern{white, black}};

    EXPECT_EQ(pattern_at(pattern, pnt3{0, 0, 0}), white);
    EXPECT_EQ(pattern_at(pattern, pnt3{0.9, 0, 0}), white);
    EXPECT_EQ(pattern_at(pattern, pnt3{1, 0, 0}), black);
    EXPECT_EQ(pattern_at(pattern, pnt3{-0.1, 0, 0}), black);
    EXPECT_EQ(pattern_at(pattern, pnt3{-1, 0, 0}), black);
    EXPECT_EQ(pattern_at(pattern, pnt3{-1.1, 0, 0}), white);
}

TEST_F(PatternTest, YAxis) {
    pattern pattern = stripe_pattern{white, black};

    EXPECT_EQ(pattern_at(pattern, pnt3{0, 0, 0}), white);
    EXPECT_EQ(pattern_at(pattern, pnt3{0, 1, 0}), white);
    EXPECT_EQ(pattern_at(pattern, pnt3{0, 2, 0}), white);
}

TEST_F(PatternTest, ZAxis) {
    pattern pattern = stripe_pattern{white, black};

    EXPECT_EQ(pattern_at(pattern, pnt3{0, 0, 0}), white);
    EXPECT_EQ(pattern_at(pattern, pnt3{0, 0, 1}), white);
    EXPECT_EQ(pattern_at(pattern, pnt3{0, 0, 2}), white);
}

TEST_F(PatternTest, WithShapeTform) {
    shape s{sphere{scale(2, 2, 2)}};
    pattern pattern = stripe_pattern{white, black};

    color c = pattern_at(pattern, s, {1.5, 0, 0});

    EXPECT_EQ(c, white);
}

TEST_F(PatternTest, WithPatternTform) {
    shape s{sphere{}};
    pattern pattern = stripe_pattern{white, black, {scale(2, 2, 2)}};

    color c = pattern_at(pattern, s, {1.5, 0, 0});

    EXPECT_EQ(c, white);
}

TEST_F(PatternTest, WithShapeAndPatternTform) {
    shape s{sphere{scale(2, 2, 2)}};
    pattern pattern = stripe_pattern{white, black, {tform4::translate({.5, 0, 0})}};

    color c = pattern_at(pattern, s, {2.5, 0, 0});

    EXPECT_EQ(c, white);
}

TEST_F(PatternTest, Gradient) {
    pattern pattern{gradient_pattern{white, black}};

    EXPECT_EQ(pattern_at(pattern, {0, 0, 0}), white);
    EXPECT_EQ(pattern_at(pattern, {.25, 0, 0}), (color{.75, .75, .75}));
    EXPECT_EQ(pattern_at(pattern, {.5, 0, 0}), (color{.5, .5, .5}));
    EXPECT_EQ(pattern_at(pattern, {.75, 0, 0}), (color{.25, .25, .25}));
}

TEST_F(PatternTest, Ring) {
    pattern pattern{ring_pattern{white, black}};

    EXPECT_EQ(pattern_at(pattern, {0, 0, 0}), white);
    EXPECT_EQ(pattern_at(pattern, {1, 0, 0}), black);
    EXPECT_EQ(pattern_at(pattern, {0, 0, 1}), black);
    EXPECT_EQ(pattern_at(pattern, {.708, 0, .708}), black); // > sqrt2_v / 2
}
