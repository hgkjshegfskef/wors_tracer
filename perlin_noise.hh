#pragma once

namespace wt {

float perlin_noise(float x, float y, float z) noexcept;
float octave_perlin_noise(float x, float y, float z, int octaves, float persistence) noexcept;

} // namespace wt
