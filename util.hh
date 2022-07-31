#include <string_view>
#include <utility>

namespace wt {

struct color;

// https://en.wikipedia.org/wiki/Linear_interpolation
// If the two known points are given by the coordinates (x0,y0) and (x1,y1), the linear interpolant
// is the straight line between these points. If the value of x is in the interval [x0;x1], the
// function interpolates, otherwise, the function extrapolates.
float lerp(float x, std::pair<float, float> left_point,
           std::pair<float, float> right_point) noexcept;

void write_ppm(char const* fname, std::string_view contents) noexcept;

float scale(float value, std::pair<float, float> source_range,
            std::pair<float, float> target_range) noexcept;

float clamp_and_scale(color const& col, unsigned component_idx) noexcept;

} // namespace wt
