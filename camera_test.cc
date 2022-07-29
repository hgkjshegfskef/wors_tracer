#include "camera.hh"
#include "mat3.hh"
#include "pnt3.hh"
#include "ray.hh"
#include "tform4.hh"
#include "vec3.hh"

#include <gtest/gtest.h>

#include <numbers>

using namespace wt;
using namespace std::numbers;

TEST(CameraTest, DefaultOrientation) {
    pnt3 from{0, 0, 0};
    pnt3 to{0, 0, -1};
    vec3 up{0, 1, 0};

    tform4 orientation = view(from, to, up);
    tform4 expected; // identity

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            EXPECT_EQ((orientation(i, j)), (expected(i, j)));
        }
    }
}

TEST(CameraTest, PositiveZ) {
    pnt3 from{0, 0, 0};
    pnt3 to{0, 0, 1};
    vec3 up{0, 1, 0};

    tform4 orientation = view(from, to, normalize(up));
    tform4 expected{scale(-1, 1, -1)};

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            EXPECT_EQ((orientation(i, j)), (expected(i, j))) << "i,j: " << i << "," << j;
        }
    }
}

TEST(CameraTest, Translation) {
    pnt3 from{0, 0, 8};
    pnt3 to{0, 0, 1};
    vec3 up{0, 1, 0};

    tform4 orientation = view(from, to, normalize(up));
    tform4 expected;
    expected.set_translation(pnt3{0, 0, -8});

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            EXPECT_EQ((orientation(i, j)), (expected(i, j)));
        }
    }
}

TEST(CameraTest, Arbitrary) {
    pnt3 from{1, 3, 2};
    pnt3 to{4, -2, 8};
    vec3 up{1, 1, 0};

    tform4 orientation = view(from, to, normalize(up));
    tform4 expected{-0.50709f, 0.50709f, 0.67612f,  -2.36643f, //
                    0.76772f,  0.60609f, 0.12122f,  -2.82843f, //
                    -0.35857f, 0.59761f, -0.71714f, 0.f};

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            EXPECT_NEAR((orientation(i, j)), (expected(i, j)), 1e-5f) << "i,j: " << i << "," << j;
        }
    }
}

TEST(CameraTest, Camera) {
    camera cam{160, 120, pi_v<float> / 2};
    tform4 expected; // identity

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            EXPECT_EQ((cam.tform(i, j)), (expected(i, j)));
        }
    }
}

TEST(CameraTest, HorizontalCanvas) {
    camera cam{200, 125, pi_v<float> / 2};
    EXPECT_EQ(cam.pixel_size, .01f);
}

TEST(CameraTest, VerticalCanvas) {
    camera cam{125, 200, pi_v<float> / 2};
    EXPECT_EQ(cam.pixel_size, .01f);
}

TEST(CameraTest, RayThroughCanvasCenter) {
    camera cam{201, 101, pi_v<float> / 2};

    ray r = ray_for_pixel(cam, <#initializer #>, 100, 50);

    EXPECT_EQ(r.origin.x, 0);
    EXPECT_EQ(r.origin.y, 0);
    EXPECT_EQ(r.origin.z, 0);
    EXPECT_NEAR(r.direction.x, 0.f, 1e-5f);
    EXPECT_NEAR(r.direction.y, 0.f, 1e-5f);
    EXPECT_NEAR(r.direction.z, -1.f, 1e-5f);
}

TEST(CameraTest, RayThroughCanvasCorner) {
    camera cam{201, 101, pi_v<float> / 2};

    ray r = ray_for_pixel(cam, <#initializer #>, 0, 0);

    EXPECT_EQ(r.origin.x, 0);
    EXPECT_EQ(r.origin.y, 0);
    EXPECT_EQ(r.origin.z, 0);
    EXPECT_NEAR(r.direction.x, .66519f, 1e-5f);
    EXPECT_NEAR(r.direction.y, .33259f, 1e-5f);
    EXPECT_NEAR(r.direction.z, -0.66851f, 1e-5f);
}

TEST(CameraTest, RayThroughTransformedCamera) {
    camera cam{201, 101, pi_v<float> / 2};
    tform4 tran;
    tran.set_translation({0, -2, 5});
    tform4 rot{rotation<Axis::Y>(pi_v<float> / 4)};
    cam.tform = rot * tran;

    ray r = ray_for_pixel(cam, <#initializer #>, 100, 50);

    EXPECT_NEAR(r.origin.x, 0.f, 1e-5f);
    EXPECT_NEAR(r.origin.y, 2.f, 1e-5f);
    EXPECT_NEAR(r.origin.z, -5.f, 1e-5f);
    EXPECT_NEAR(r.direction.x, sqrt2_v<float> / 2, 1e-5f);
    EXPECT_NEAR(r.direction.y, 0, 1e-5f);
    EXPECT_NEAR(r.direction.z, -sqrt2_v<float> / 2, 1e-5f);
}
