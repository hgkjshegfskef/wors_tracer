#include "camera.hh"
#include "canvas.hh"
#include "cli.hh"
#include "log.hh"
#include "render.hh"
#include "scene.hh"

#include <CLI/CLI.hpp>

#include <spdlog/spdlog.h>

#include <functional>
#include <vector>

#define SCENE(n) scene_##n

using namespace wt;

int main(int argc, char** argv) {
    setup_logging();

    CLI::App app{"worst racer"};
    app.set_help_flag("--help", "Print this help message and exit");

    cli cli;

    app.add_option("-r,--render-backend", cli.render_backend, "Where to dump the pixels")
        ->default_val(cli.render_backend)
        ->check(CLI::IsMember{{"ppm", "sdl"}});

    app.add_option("--win-width", cli.win_width, "SDL window width")->default_val(cli.win_width);
    app.add_option("--win-height", cli.win_height, "SDL window height")
        ->default_val(cli.win_height);

    app.add_option("--tex-width", cli.tex_width, "SDL texture width")->default_val(cli.tex_width);
    app.add_option("--tex-height", cli.tex_height, "SDL texture height")
        ->default_val(cli.tex_height);

    app.add_option("-s,--scene", cli.scene, "Scene number")->default_val(cli.scene);

    app.add_option("ppm-file", cli.ppm_fname, "PPM file name")->default_val("image.ppm");

    app.add_flag("--vsync,!--no-vsync", cli.vsync, "Use vertical synchronization")
        ->default_val(cli.vsync);

    CLI11_PARSE(app, argc, argv);

    std::vector<std::function<scene(struct cli const&)>> scenes{
        SCENE(0), SCENE(1), SCENE(2), SCENE(3), SCENE(4), SCENE(5), SCENE(6), SCENE(7), SCENE(8)};
    scene scene;
    try {
        scene = scenes.at(cli.scene)(cli);
    } catch (std::exception const& ex) {
        SPDLOG_ERROR("Incorrect scene number.");
        std::exit(1);
    }

    if (cli.render_backend == "sdl") {
        render_sdl(cli, scene.world, scene.look_at, scene.camera);
    } else if (cli.render_backend == "ppm") {
        canvas image{scene.camera.hsize, scene.camera.vsize};
        render_ppm(cli, scene, image);
    } else {
        SPDLOG_ERROR("Incorrect rendering backend: {}", cli.render_backend);
        return 1;
    }
}
