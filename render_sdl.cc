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

#include <chrono> // steady_clock
#include <cmath>
#include <cstdlib> //atexit
#include <vector>

using namespace oneapi;

namespace wt {

void render_sdl(camera& camera, world const& world) noexcept {
    if (std::atexit(SDL_Quit)) {
        SPDLOG_ERROR("atexit() registration failed");
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SPDLOG_ERROR("SDL_Init error: {}", SDL_GetError());
        return;
    }

    auto free_window = [](SDL_Window* w) { SDL_DestroyWindow(w); };
    std::unique_ptr<SDL_Window, decltype(free_window)> window{
        SDL_CreateWindow("worst racer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                         camera.hsize, camera.vsize, 0),
        std::move(free_window)};
    if (!window) {
        SPDLOG_ERROR("SDL_CreateWindow error: {}", SDL_GetError());
        return;
    }

    auto free_surface = [](SDL_Surface* s) { SDL_FreeSurface(s); };
    std::unique_ptr<SDL_Surface, decltype(free_surface)> surface{SDL_GetWindowSurface(window.get()),
                                                                 std::move(free_surface)};
    if (!surface) {
        SPDLOG_ERROR("SDL_GetWindowSurface error: {}", SDL_GetError());
        return;
    }

    SPDLOG_INFO("Pixel format: {}", SDL_GetPixelFormatName(surface->format->format));

    unsigned frame_cnt = 0;

    pnt3 from{0, 1.5f, -5};
    vec3 up{0, 1, 0};
    vec3 forward{0, 0, 1};
    vec3 back = -forward;
    vec3 left = normalize(cross(forward, up));
    vec3 right = -left;

    Uint64 delta_time = 0;
    Uint64 last_frame = 0;

    while (true) {
        Uint64 const current_frame = SDL_GetTicks64();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;
        float const cam_speed = 0.0025f * delta_time;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                return;
            }
        }

        auto* state = SDL_GetKeyboardState(nullptr);
        if (state[SDL_SCANCODE_ESCAPE]) {
            return;
        }
        if (state[SDL_SCANCODE_W]) {
            from = position(from, forward, cam_speed);
        }
        if (state[SDL_SCANCODE_S]) {
            from = position(from, back, cam_speed);
        }
        if (state[SDL_SCANCODE_A]) {
            from = position(from, left, cam_speed);
        }
        if (state[SDL_SCANCODE_D]) {
            from = position(from, right, cam_speed);
        }

        //        float const radius = 3.f;
        //        float cam_x = std::cos(SDL_GetTicks64() / 1000.f) * radius;
        //        float cam_z = std::sin(SDL_GetTicks64() / 1000.f) * radius;

        camera.tform = v3::view(from, forward, up);
        tform4 const inv_cam_tform = inverse(camera.tform);

        //        SDL_memset(surface->pixels, 0, surface->h * surface->pitch);

        auto start = std::chrono::steady_clock::now();

        tbb::parallel_for(
            tbb::blocked_range2d<int>(0, camera.vsize, 0, camera.hsize), [&](auto const range) {
                std::vector<intersection> world_isecs;
                world_isecs.reserve(world.shapes.size() * 2);
                auto* pixels = reinterpret_cast<unsigned*>(surface->pixels);
                for (int y = range.rows().begin(); y != range.rows().end(); ++y) {
                    for (int x = range.cols().begin(); x != range.cols().end(); ++x) {
                        color const color = color_at(
                            world, ray_for_pixel(camera, inv_cam_tform, x, y), world_isecs);
                        pixels[y * camera.hsize + x] = SDL_MapRGB(
                            surface->format, clamp_and_scale(color, 0) + .5f,
                            clamp_and_scale(color, 1) + .5f, clamp_and_scale(color, 2) + .5f);
                    }
                }
            });

        auto stop = std::chrono::steady_clock::now();

        SDL_UpdateWindowSurface(window.get());

        SPDLOG_INFO("Frame #{} drawn in {}", ++frame_cnt,
                    std::chrono::duration<double, std::milli>(stop - start));
    }
}

} // namespace wt
