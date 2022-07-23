#include "ray.hh"
#include "sphere.hh"

#include <gtest/gtest.h>

using namespace wt;

TEST(SphereTest, Ctor) {
    auto scale =
        std::make_shared<tform4>(vec3{2, 0, 0}, vec3{0, 2, 0}, vec3{0, 0, 2}, pnt3{0, 0, 0});
    sphere s{scale};
}

TEST(SphereTest, Intersect1) {
    // no transformation
    pnt3 const ray_origin = pnt3{-5.f, 0.f, 0.f};
    pnt3 const sphere_origin = pnt3{0.f, 0.f, 0.f};
    ray r{ray_origin, normalize(sphere_origin - ray_origin)};
    sphere s{std::make_shared<tform4>(tform4::identity())};

    auto i = intersect(r, s);
}

TEST(SphereTest, Intersect2) {
    pnt3 const ray_origin = pnt3{-5.f, 0.f, 0.f};
    pnt3 const sphere_origin = pnt3{0.f, 0.f, 0.f};
    ray r{ray_origin, normalize(sphere_origin - ray_origin)};
    sphere s{std::make_shared<tform4>(2.f, 0.f, 0.f, 0.f, //
                                      0.f, 2.f, 0.f, 0.f, //
                                      0.f, 0.f, 2.f, 0.f)};

    auto i = intersect(r, s);
}
