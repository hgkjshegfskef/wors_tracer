#pragma once

#include <string>

namespace wt {

struct cli {
    std::string render_backend{"sdl"};
    unsigned width{1920};
    unsigned height{1080};
    unsigned scene{1};
    std::string ppm_fname{"image.ppm"};
};

} // namespace wt
