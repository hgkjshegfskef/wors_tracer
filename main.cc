#include "camera.hh"
#include "cli.hh"
#include "log.hh"
#include "render.hh"
#include "scene.hh"
#include "shape.hh"

#include <CLI/CLI.hpp>

#include <spdlog/spdlog.h>

using namespace wt;

int main(int argc, char** argv) {
    setup_logging();

    CLI::App app{"worst racer"};
    app.set_help_flag("--help", "Print this help message and exit");

    cli cli;

    app.add_option("-r,--render-backend", cli.render_backend, "Where to dump the pixels")
        ->check(CLI::IsMember{{"ppm", "sdl"}})
        ->default_val("sdl")
        ->run_callback_for_default();

    app.add_option("--win-width", cli.win_width, "SDL window width")
        ->default_val(1920)
        ->run_callback_for_default();
    app.add_option("--win-height", cli.win_height, "SDL window height")
        ->default_val(1080)
        ->run_callback_for_default();

    app.add_option("--tex-width", cli.tex_width, "SDL texture width")
        ->default_val(960)
        ->run_callback_for_default();
    app.add_option("--tex-height", cli.tex_height, "SDL texture height")
        ->default_val(540)
        ->run_callback_for_default();

    app.add_option("-s,--scene", cli.scene, "Scene number")
        ->default_val(1)
        ->run_callback_for_default();

    app.add_option("ppm-file", cli.ppm_fname, "PPM file name")
        ->default_val("image.ppm")
        ->run_callback_for_default();

    CLI11_PARSE(app, argc, argv);

    scene scene = get_scene(cli.scene, cli);

    if (cli.render_backend == "sdl") {
        render_sdl(scene.camera, scene.world, cli);
    } else if (cli.render_backend == "ppm") {
        render_ppm(scene.camera, scene.world);
    } else {
        SPDLOG_ERROR("Incorrect rendering backend: {}", cli.render_backend);
        return 1;
    }
}
