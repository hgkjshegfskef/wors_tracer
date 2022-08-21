#include "camera.hh"
#include "canvas.hh"
#include "render.hh"
#include "shape.hh"
#include "world.hh"

#include <gtest/gtest.h>

using namespace wt;
using namespace std::numbers;

TEST(RenderTest, DefaultWorld) {
    world w = world::make_default();
    camera cam{11, 11, pi_v<float> / 2};
    pnt3 from{0, 0, -5};
    pnt3 to{0, 0, 0};
    vec3 up{0, 1, 0};
    cam.inv_tform = inverse(view(from, to, up));

    canvas image{cam.hsize, cam.vsize};
    render_ppm(cam, w, image, "image.ppm", 1);

    EXPECT_NEAR(image(5, 5).r, .38066f, 1e-5f);
    EXPECT_NEAR(image(5, 5).g, .47583f, 1e-5f);
    EXPECT_NEAR(image(5, 5).b, .2855f, 1e-5f);
}
