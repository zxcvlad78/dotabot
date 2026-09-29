#include <meatengine/ScriptingServer.hpp>

#include <iostream>

namespace meatengine {

sol::state ScriptingServer::lua_;

static void ensure_initialized() {
    static bool inited = false;
    if (inited) return;
    inited = true;

    ScriptingServer::lua().open_libraries(
        sol::lib::base,
        sol::lib::math,
        sol::lib::string,
        sol::lib::table
    );
}

bool ScriptingServer::run_file(std::string_view path) {
    ensure_initialized();
    try {
        lua_.safe_script_file(std::string(path));
        return true;
    } catch (const sol::error& e) {
        std::cerr << "[lua] " << e.what() << '\n';
        return false;
    }
}

bool ScriptingServer::run_string(std::string_view code) {
    ensure_initialized();
    try {
        lua_.safe_script(std::string(code));
        return true;
    } catch (const sol::error& e) {
        std::cerr << "[lua] " << e.what() << '\n';
        return false;
    }
}

} // namespace meatengine