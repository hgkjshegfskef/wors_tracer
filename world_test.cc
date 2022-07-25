#include "ray.hh"
#include "world.hh"

#include <gtest/gtest.h>

using namespace wt;

TEST(WorldTest, Default) {
    world w = world::make_default();
    ray world_r{pnt3{0.f, 0.f, -5.f}, vec3{0.f, 0.f, 1.f}};

//    pnt3 const pos_on_object_r = position(object_r.origin, object_r.direction, *t);
//    pnt3 const pos_on_world_r = s.tform * pos_on_object_r;
}
