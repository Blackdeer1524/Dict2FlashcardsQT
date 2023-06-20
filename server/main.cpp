// https://github.com/gabime/spdlog/issues/1515
#include "spdlog/common.h"
#define SPDLOG_ACTIVE_LEVEL                                                    \
    SPDLOG_LEVEL_TRACE  // Must: define SPDLOG_ACTIVE_LEVEL before `#include
                        // "spdlog/spdlog.h"`
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/spdlog.h"

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <memory>
#include <random>
#include <stdexcept>
#include <string>

#include "argparse.hpp"

#include "PluginsProvider.hpp"
#include "Server.hpp"
#include "sysmodule.h"

auto main(int argc, char *argv[]) -> int {
    // https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
    spdlog::set_pattern("[%H:%M:%S] [%s] line %# %v");

    argparse::ArgumentParser program{};
    program.add_argument("--plugins-path")
        .help("A path to a directory with plugins")
        .default_value((std::filesystem::current_path() / "plugins").string());

    program.add_argument("--python-path")
        .help("A path to a Python Interpreter")
        .default_value("");

    constexpr uint16_t default_port = 8888;
    program.add_argument("-p", "--port")
        .help("A port to deploy server on")
        .default_value(default_port)
        .scan<'u', uint16_t>();

    try {
        program.parse_args(argc, argv);
    } catch (const std::runtime_error &err) {
        SPDLOG_ERROR("Couldn't parse arguments. Reason: {}", err.what());
        std::exit(1);
    }

    std::filesystem::path plugins_dir = program.get("--plugins-path");
    std::filesystem::path python_path = program.get("--python-path");
    auto                  port        = program.get<uint16_t>("--port");

    std::filesystem::path absolute_path_to_plugins_dir;
    try {
        if (!std::filesystem::exists(plugins_dir)) {
            SPDLOG_ERROR(
                "Couldn't parse a path to plugins. Reason: path is not valid");
            std::exit(1);
        }

        absolute_path_to_plugins_dir = std::filesystem::absolute(plugins_dir);
    } catch (const std::filesystem::filesystem_error &err) {
        SPDLOG_ERROR("Couldn't parse a path to plugins. Reason: {}",
                     err.what());
        std::exit(1);
    }

    if (!python_path.empty()) {
        if (!std::filesystem::exists(python_path)) {
            SPDLOG_ERROR("Couldn't parse a path to Python interpreter. Reason "
                         "path is not valid");
            std::exit(1);
        }

        std::filesystem::path absolute_path_to_python_interpreter;
        try {
            absolute_path_to_python_interpreter =
                std::filesystem::absolute(python_path);
        } catch (const std::filesystem::filesystem_error &err) {
            SPDLOG_ERROR(
                "Couldn't parse a path to Python interpreter. Reason: {}",
                err.what());
            std::exit(1);
        }
        Py_SetProgramName(absolute_path_to_python_interpreter.wstring().data());
    }
    Py_Initialize();
    auto plugins_dirs = PluginTypesLocationsConfig{
        .definitions_providers_dir =
            absolute_path_to_plugins_dir / "definitions/",
        .sentences_providers_dir = absolute_path_to_plugins_dir / "sentences/",
        .images_providers_dir    = absolute_path_to_plugins_dir / "images/",
        .audios_providers_dir    = absolute_path_to_plugins_dir / "audios/",
        .format_processors_dir =
            absolute_path_to_plugins_dir / "format_processors/"};

    boost::asio::io_context io_context;
    boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
    signals.async_wait(
        boost::bind(&boost::asio::io_service::stop, &io_context));

    auto plugins_provider =
        std::make_shared<PluginsProvider>(std::move(plugins_dirs));
    auto server = PluginServer(std::move(plugins_provider), io_context, port);
    io_context.run();
}
