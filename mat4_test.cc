#include "mat4.hh"

#include <gtest/gtest.h>

using namespace wt;

TEST(mat4test, DefaultCtor) { [[maybe_unused]] mat4 m; }

TEST(mat4test, ColumnVec) {
    mat4 m(vec4(1.f, 2.f, 3.f, 4.f), vec4(5.f, 6.f, 7.f, 8.f), vec4(9.f, 10.f, 11.f, 12.f),
           vec4(13.f, 14.f, 15.f, 16.f));

    vec4 const& v0 = m[0];
    vec4 const& v1 = m[1];
    vec4 const& v2 = m[2];
    vec4 const& v3 = m[3];

    EXPECT_NEAR(v0.x, 1.f, 1e-6);
    EXPECT_NEAR(v0.y, 2.f, 1e-6);
    EXPECT_NEAR(v0.z, 3.f, 1e-6);
    EXPECT_NEAR(v0.w, 4.f, 1e-6);

    EXPECT_NEAR(v1.x, 5.f, 1e-6);
    EXPECT_NEAR(v1.y, 6.f, 1e-6);
    EXPECT_NEAR(v1.z, 7.f, 1e-6);
    EXPECT_NEAR(v1.w, 8.f, 1e-6);

    EXPECT_NEAR(v2.x, 9.f, 1e-6);
    EXPECT_NEAR(v2.y, 10.f, 1e-6);
    EXPECT_NEAR(v2.z, 11.f, 1e-6);
    EXPECT_NEAR(v2.w, 12.f, 1e-6);

    EXPECT_NEAR(v3.x, 13.f, 1e-6);
    EXPECT_NEAR(v3.y, 14.f, 1e-6);
    EXPECT_NEAR(v3.z, 15.f, 1e-6);
    EXPECT_NEAR(v3.w, 16.f, 1e-6);
}

TEST(mat4test, Inverse) {
    mat4 m{1.f,  1.f,  1.f,  -1.f, //
           1.f,  1.f,  -1.f, 1.f,  //
           1.f,  -1.f, 1.f,  1.f,  //
           -1.f, 1.f,  1.f,  1.f};

    auto inv_m = inverse(m);

    mat4 expected{.25f,  .25f,  .25f,  -.25f, //
                  .25f,  .25f,  -.25f, .25f,  //
                  .25f,  -.25f, .25f,  .25f,  //
                  -.25f, .25f,  .25f,  .25f};

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            EXPECT_NEAR(inv_m[i][j], expected[i][j], 1e-6) << "i=" << i << ", j=" << j;
        }
    }
}
