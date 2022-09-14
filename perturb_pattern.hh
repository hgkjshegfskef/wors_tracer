#pragma once

#include "pattern.hh"
#include "tform4.hh"

namespace wt {

struct perturb_pattern {
    class pattern pattern;
    int octaves;       // [1;inf] for octave-based Perlin noise
    float persistence; // [1;inf] for octave-based Perlin noise
    float scale;       // [0;1] how much weight the jitter has
    tform4 tform;
    tform4 inv_tform;

    explicit perturb_pattern(class pattern pattern, float scale = 1.f, int octaves = 1,
                             float persistence = 1.f) noexcept;
    perturb_pattern(class pattern pattern, tform4 tform, float scale = 1.f, int octaves = 1,
                    float persistence = 1.f) noexcept;
};

} // namespace wt
