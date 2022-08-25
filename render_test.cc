#include "camera.hh"
#include "canvas.hh"
#include "cli.hh"
#include "look_at.hh"
#include "render.hh"
#include "scene.hh"
#include "shape.hh"
#include "world.hh"

#include <gtest/gtest.h>

using namespace wt;
using namespace std::numbers;

TEST(RenderTest, DefaultWorld) {
    world w = world::make_default();
    look_at look_at{{0, 0, -5}, {0, 0, 0}, {0, 1, 0}};
    camera cam{11, 11, pi_v<float> / 2, look_at};

    cli cli;
    scene scene{w, cam, look_at};
    canvas image{cam.hsize, cam.vsize};
    render_ppm(cli, scene, image);

    EXPECT_NEAR(image(5, 5).r, .38066f, 1e-5f);
    EXPECT_NEAR(image(5, 5).g, .47583f, 1e-5f);
    EXPECT_NEAR(image(5, 5).b, .2855f, 1e-5f);
}
