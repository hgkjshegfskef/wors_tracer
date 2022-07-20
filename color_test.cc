#include "color.hh"

#include <fmt/core.h>
#include <gtest/gtest.h>

#include <algorithm> // for_each
#include <string>    // string

using namespace wt;

TEST(ColorTest, DefaultPrint) {
    color c{1.f, 2.f, 3.f};
    std::string result = fmt::format("{}", c);
    std::string expected{"1 2 3"};
    EXPECT_EQ(result, expected);
}

TEST(ColorTest, CustomPrint) {
    color c{1.23f, 2.34f, 3.333f};
    std::string result = fmt::format("{:3.0f}", c);
    std::string expected{"  1   2   3"};
    EXPECT_EQ(result, expected);
}

TEST(ColorTest, CustomPrint2) {
    color c{255.f, 255.f, 255.f};
    std::string result = fmt::format("{:3.0f}", c);
    std::string expected{"255 255 255"};
    EXPECT_EQ(result, expected);
}

TEST(ColorTest, Iterators) {
    color c{1.f, 2.f, 3.f};
    float expected = 1.f;
    std::for_each(c.cbegin(), c.cend(), [&](auto val) { EXPECT_EQ(val, expected++); });
}
