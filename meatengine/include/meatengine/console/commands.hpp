#pragma once

#include <entt/entt.hpp>
#include <SFML/Audio.hpp>
#include <meatengine/meatengine.hpp>


namespace meatengine::console_commands {
    inline void init(meatengine::MainLoop& main_loop) {
        Console::get_instance().register_command(
            "exit",
            [&main_loop](const std::vector<std::string>& args) {
                main_loop.get_window().close();
            },
            "Close window and exit",
            "exit"
        );
        Console::get_instance().register_command(
            "echo.mode",
            [](const std::vector<std::string>& args) {
                if (args.empty()) {
                    Console::get_instance().print("echo mode: " + std::to_string(Console::get_instance().echo_mode));
                    return;
                }
                try {
                    Console::get_instance().echo_mode = std::stoi(args[0]);

                } catch (const std::exception& e) {
                    Console::get_instance().print_error(e.what());
                }
            },
            "Echo mode",
            "echo, echo <bool>"
        );
        Console::get_instance().register_command(
            "echo",
            [](const std::vector<std::string>& args) {
                std::string total_string;
                for (auto c : args) {
                    total_string += c + " ";
                }

                try {
                    Console::get_instance().print(total_string);
                } catch (const std::exception& e) {
                    Console::get_instance().print_error(e.what());
                }
            },
            "Echo message",
            "echo <string>"
        );
        Console::get_instance().register_command(
            "cfg.save",
            [](const std::vector<std::string>& args) {
                Console::get_instance().config_file->save();
            },
            "Save cfg file",
            "cfg.save"
        );
        Console::get_instance().register_command(
            "cfg.load",
            [&main_loop](const std::vector<std::string>& args) {
                Console::get_instance().load_cfg(main_loop.get_window());
            },
            "Load cfg file",
            "cfg.load"
        );
        Console::get_instance().register_command(
            "cfg.reset",
            [&main_loop](const std::vector<std::string>& args) {
                Console::get_instance().reset_cfg(main_loop.get_window());
            },
            "Reset cfg file",
            "cfg.reset"
        );
        Console::get_instance().register_command(
            "fps.max",
            [&main_loop](const std::vector<std::string>& args) {
                if (!args.empty()) {
                    try {
                        int fps = std::stoi(args[0]);
                        main_loop.set_framerate_limit(fps);
                    } catch (const std::exception& e) {
                        Console::get_instance().print_error(e.what());
                    }
                }
            },
            "Set target framerate",
            "fps.max <int>"
        );
        Console::get_instance().register_command(
            "volume",
            [](const std::vector<std::string>& args) {
                if (!args.empty()) {
                    try {
                        //float vol = std::stoi(args[0]);
                        sf::Listener::setGlobalVolume(std::stoi(args[0]));
                    } catch (const std::exception& e) {
                        Console::get_instance().print_error(e.what());
                    }
                }
            },
            "Set audio volume (0.0-100.0)",
            "volume <float>"
        );
        Console::get_instance().register_command(
            "speed",
            [](const std::vector<std::string>& args) {
                if (!args.empty()) {
                    try {
                        float val = std::stof(args[0]);
                        if (val >= 0.f) {
                            meatengine::MainLoop::dt_scale = val;
                            Console::get_instance().print_success("Speed scale set to: " + std::to_string(val));
                        } else {
                            Console::get_instance().print_error("Speed scale must be positive");
                        }
                    } catch (const std::exception& e) {
                        Console::get_instance().print_error(e.what());
                    }
                } else {
                    Console::get_instance().print_success("Current speed scale: " + std::to_string(meatengine::MainLoop::dt_scale));
                }
            },
            "Set time speed multiplier",
            "speed <float>"
        );

        // Test
        // for (uint8_t i = 0; i < 25; i ++) {
        //     std::string str_i = std::to_string(i);
        //     Console::get_instance().register_command(
        //         "sas" + str_i,
        //         [](const std::vector<std::string>& args) { },
        //         "pro100 sas nomer " + str_i,
        //         "nikak))"
        //     );
        // }
    }
}