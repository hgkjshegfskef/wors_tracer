#include "camera.hh"
#include "intersection.hh"
#include "ray.hh"
#include "render.hh"
#include "shape.hh"
#include "util.hh"
#include "world.hh"

#include <SDL2/SDL.h>
#include <oneapi/tbb/blocked_range2d.h>
#include <oneapi/tbb/parallel_for.h>

#include <fmt/chrono.h>
#include <spdlog/spdlog.h>

#include <chrono>  // steady_clock
#include <cstdlib> //atexit
#include <vector>

using namespace oneapi;

namespace wt {

void render_sdl(camera const& camera, world const& world) noexcept {
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

    tform4 const inv_cam_tform = inverse(camera.tform);
    unsigned frame_cnt = 0;

    while (true) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                SDL_Quit();
                std::exit(0);
            }
        }

        SDL_memset(surface->pixels, 0, surface->h * surface->pitch);


        auto start = std::chrono::steady_clock::now();

        //        tbb::parallel_for(
        //            tbb::blocked_range2d<int>(0, camera.vsize, 0, camera.hsize), [&](auto const
        //            range) {
        //                std::vector<intersection> world_isecs;
        //                world_isecs.reserve(world.shapes.size() * 2);
        //                auto* pixels = reinterpret_cast<unsigned*>(surface->pixels);
        //                for (int y = range.rows().begin(); y != range.rows().end(); ++y) {
        //                    for (int x = range.cols().begin(); x != range.cols().end(); ++x) {
        //                        color const color = color_at(
        //                            world, ray_for_pixel(camera, inv_cam_tform, x, y),
        //                            world_isecs);
        //                        pixels[y * camera.hsize + x] = SDL_MapRGB(
        //                            surface->format, clamp_and_scale(color, 0) + .5f,
        //                            clamp_and_scale(color, 1) + .5f, clamp_and_scale(color, 2) +
        //                            .5f);
        //                    }
        //                }
        //            });

                std::vector<intersection> world_isecs;
                world_isecs.reserve(world.shapes.size() * 2);
                auto* pixels = reinterpret_cast<unsigned*>(surface->pixels);
                for (int y = 0; y != camera.vsize; ++y) {
                    for (int x = 0; x != camera.hsize; ++x) {
                        color const color = color_at(
                            world, ray_for_pixel(camera, inv_cam_tform, x, y), world_isecs);
                        pixels[y * camera.hsize + x] = SDL_MapRGB(
                            surface->format, clamp_and_scale(color, 0) + .5f,
                            clamp_and_scale(color, 1) + .5f, clamp_and_scale(color, 2) + .5f);
                    }
                }

        auto stop = std::chrono::steady_clock::now();

        SDL_UpdateWindowSurface(window.get());

        // This triggers UB:
        //        SPDLOG_INFO("Frame #{} drawn in {}", ++frame_cnt,
        //                    std::chrono::duration<double, std::milli>(stop - start));
        // But this doesn't:
        //        SPDLOG_INFO("Frame #{} drawn in {}", ++frame_cnt,
        //                    std::chrono::duration_cast<std::chrono::milliseconds>(stop - start));
    }

    SDL_Quit();
}

} // namespace wt
