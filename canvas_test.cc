#include "canvas.hh"
#include "color.hh"

#include <gtest/gtest.h>

using namespace wt;

TEST(CanvasTest, Ctor) { canvas c(100, 100); }

TEST(CanvasTest, ExportPpmSimple) {
    canvas c1{5, 3};
    c1.fill({0, 0, 0});

    color col1{1.5, 0, 0};
    color col2{0, 0.5, 0};
    color col3{-0.5, 0, 1};

    // flip Y axis
    c1(0, 2 - 2) = col1;
    c1(2, 2 - 1) = col2;
    c1(4, 2 - 0) = col3;

    std::string const image = c1.as_ppm();
    std::string_view expected{"P3\n"
                              "5 3\n"
                              "255\n"
                              "255   0   0   0   0   0   0   0   0   0   0   0   0   0   0\n"
                              "  0   0   0   0   0   0   0 128   0   0   0   0   0   0   0\n"
                              "  0   0   0   0   0   0   0   0   0   0   0   0   0   0 255\n"};
    EXPECT_EQ(image, expected) << image;
}

TEST(CanvasTest, ExportPpmLongLines) {
    canvas c1{10, 2};
    color col1{1, 0.8, 0.6};
    c1.fill(col1);

    std::string const image = c1.as_ppm();
    std::string expected{"P3\n"
                         "10 2\n"
                         "255\n"
                         "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153\n"
                         "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153\n"
                         "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153\n"
                         "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153\n"};
    EXPECT_EQ(image, expected) << image;
}

TEST(CanvasTest, ExportPpmTail) {
    canvas c1{3, 6};
    color col1{1, 0.8, 0.6};
    c1.fill(col1);

    const std::string image = c1.as_ppm();
    std::string expected{"P3\n"
                         "3 6\n"
                         "255\n"
                         "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153\n"
                         "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153\n"
                         "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153\n"
                         "255 204 153 255 204 153 255 204 153\n"};
    EXPECT_EQ(image, expected) << image;
}
