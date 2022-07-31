#include "render.hh"

#include "camera.hh"
#include "canvas.hh"
#include "intersection.hh"
#include "ray.hh"
#include "shape.hh"
#include "util.hh"
#include "world.hh"

#include <SDL2/SDL.h>
#include <oneapi/tbb/blocked_range2d.h>
#include <oneapi/tbb/parallel_for.h>

#include <spdlog/spdlog.h>

#include <cstdlib>
#include <mutex>
#include <vector>

using namespace oneapi;

namespace wt {

canvas render(camera const& camera, world const& world) noexcept {
    canvas image{camera.hsize, camera.vsize};
    image.fill({0, 0, 0});
    tform4 const inv_cam_tform = inverse(camera.tform);
    //    int counter = 0;
    //    std::mutex m;

    tbb::parallel_for(
        tbb::blocked_range2d<int>(0, camera.vsize, 0, camera.hsize), [&](auto const range) {
            std::vector<intersection> world_isecs;
            world_isecs.reserve(world.shapes.size() * 2);
            //            {
            //                std::scoped_lock l{m};
            //                ++counter;
            //            }
            //            SPDLOG_INFO("Processing {}x{} pixels", range.rows().end() -
            //            range.rows().begin(),
            //                        range.cols().end() - range.cols().begin());
            for (int y = range.rows().begin(); y != range.rows().end(); ++y) {
                for (int x = range.cols().begin(); x != range.cols().end(); ++x) {
                    image(x, y) =
                        color_at(world, ray_for_pixel(camera, inv_cam_tform, x, y), world_isecs);
                }
            }
        });
    //    SPDLOG_INFO("Total workers: {}", counter);

    //    std::vector<intersection> world_isecs;
    //    world_isecs.reserve(world.shapes.size() * 2);
    //    for (int y = camera.vsize - 1; y >= 0; --y) {
    //        for (unsigned x = 0; x < camera.hsize - 1; ++x) {
    //            // gradient test:
    //            //                        float const xoffset = (x + 0.5f) * camera.pixel_size;
    //            //                        float const yoffset = (y + 0.5f) * camera.pixel_size;
    //            //                        float const world_x = camera.half_width - xoffset;
    //            //                        float const world_y = camera.half_height - yoffset;
    //            //                        image(x, y) = color{world_x, world_y, 0};
    //
    //            image(x, y) = color_at(world, ray_for_pixel(camera, inv_cam_tform, x, y),
    //            world_isecs);
    //        }
    //    }

    return image;
}

void render_sdl(camera const& camera, world const& world) noexcept {
    tform4 const inv_cam_tform = inverse(camera.tform);

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SPDLOG_ERROR("SDL_Init error: {}", SDL_GetError());
        SDL_Quit();
        std::exit(1);
    }

    auto free_window = [](SDL_Window* w) { SDL_DestroyWindow(w); };
    std::unique_ptr<SDL_Window, decltype(free_window)> window{
        SDL_CreateWindow("worst racer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                         camera.hsize, camera.vsize, 0),
        std::move(free_window)};
    if (!window) {
        SPDLOG_ERROR("SDL_CreateWindow error: {}", SDL_GetError());
        SDL_Quit();
        std::exit(1);
    }

    auto free_surface = [](SDL_Surface* s) { SDL_FreeSurface(s); };
    std::unique_ptr<SDL_Surface, decltype(free_surface)> surface{SDL_GetWindowSurface(window.get()),
                                                                 std::move(free_surface)};
    if (!surface) {
        SPDLOG_ERROR("SDL_GetWindowSurface error: {}", SDL_GetError());
        SDL_Quit();
        std::exit(1);
    }

    SPDLOG_INFO("Pixel format: {}", SDL_GetPixelFormatName(surface->format->format));

    while (true) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                SDL_Quit();
                std::exit(0);
            }
        }

        std::vector<intersection> world_isecs;
        world_isecs.reserve(world.shapes.size() * 2);
        auto* pixels = reinterpret_cast<unsigned*>(surface->pixels);
        for (unsigned y = 0; y < camera.vsize; ++y) {
            for (unsigned x = 0; x < camera.hsize; ++x) {
                color const color =
                    color_at(world, ray_for_pixel(camera, inv_cam_tform, x, y), world_isecs);
                pixels[(camera.vsize - y - 1) * camera.hsize + x] =
                    SDL_MapRGBA(surface->format, clamp_and_scale(color, 0),
                                clamp_and_scale(color, 1), clamp_and_scale(color, 2), 255);
            }
        }

        SDL_UpdateWindowSurface(window.get());
    }

    SDL_Quit();
}

} // namespace wt
