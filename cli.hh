#pragma once

#include <string>

namespace wt {

struct cli {
    std::string render_backend{"sdl"};
    unsigned win_width{1920};
    unsigned win_height{1080};
    unsigned tex_width{960};
    unsigned tex_height{540};
    unsigned scene{1};
    std::string ppm_fname{"image.ppm"};
    bool vsync{false};
};

} // namespace wt
