#include "ray.hh"
#include "sphere.hh"

#include <gtest/gtest.h>

using namespace wt;

namespace {

// this repeates some code in intersect(), since the returned t is the parameter of the transformed
// ray, and to get the point coordinates in world space, we need to transform the coordinates back
pnt3 point_in_world_space(const ray& r, const sphere& s, float t) {
    tform4 const inv = inverse(s.tform);
    ray const transformed{inv * r.origin, normalize(inv * r.direction)};
    return s.tform * position(transformed.origin, transformed.direction, t);
}

} // namespace

TEST(SphereTest, Ctor) {
    auto scale = tform4(vec3{2, 0, 0}, vec3{0, 2, 0}, vec3{0, 0, 2}, pnt3{0, 0, 0});
    sphere s{std::move(scale)};
}

TEST(SphereTest, IntersectNoTransformation) {
    pnt3 const ray_origin{-5.f, 0.f, 0.f};
    pnt3 const sphere_origin{0.f, 0.f, 0.f};
    ray r{ray_origin, normalize(sphere_origin - ray_origin)};
    sphere s;

    auto i = intersect(r, s);
    EXPECT_TRUE(i);

    pnt3 p = position(r.origin, r.direction, *i);
    EXPECT_NEAR(p.x, -1.f, 1e-6f);
    EXPECT_NEAR(p.y, 0.f, 1e-6f);
    EXPECT_NEAR(p.z, 0.f, 1e-6f);
}

TEST(SphereTest, IntersectScale) {
    pnt3 const ray_origin{-5.f, 0.f, 0.f};
    pnt3 const sphere_origin{0.f, 0.f, 0.f};
    ray r{ray_origin, normalize(sphere_origin - ray_origin)};
    sphere s{tform4(2.f, 0.f, 0.f, 0.f, //
                    0.f, 2.f, 0.f, 0.f, //
                    0.f, 0.f, 2.f, 0.f)};

    auto t = intersect(r, s);
    EXPECT_TRUE(t);
    pnt3 const p = point_in_world_space(r, s, *t);
    EXPECT_NEAR(p.x, -2.f, 1e-6f);
    EXPECT_NEAR(p.y, 0.f, 1e-6f);
    EXPECT_NEAR(p.z, 0.f, 1e-6f);
}

TEST(SphereTest, IntersectTranslate) {
    pnt3 const ray_origin{-5.f, 0.f, 0.f};
    pnt3 const sphere_origin{0.f, 0.f, 0.f};
    ray r{ray_origin, normalize(sphere_origin - ray_origin)};
    sphere s{tform4(1.f, 0.f, 0.f, 2.f, //
                    0.f, 1.f, 0.f, 1.f, //
                    0.f, 0.f, 1.f, 0.f)};

    auto t = intersect(r, s);
    EXPECT_TRUE(t);
    pnt3 const p = point_in_world_space(r, s, *t);
    EXPECT_NEAR(p.x, 2.f, 1e-6f);
    EXPECT_NEAR(p.y, 0.f, 1e-6f);
    EXPECT_NEAR(p.z, 0.f, 1e-6f);
}
