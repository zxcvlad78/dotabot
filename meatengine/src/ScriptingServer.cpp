#include <meatengine/ScriptingServer.hpp>

#include <iostream>

namespace meatengine {

ScriptingServer::ScriptingServer() {
    lua_.open_libraries(sol::lib::base,
                        sol::lib::math,
                        sol::lib::string,
                        sol::lib::table);

    // lua_.set_function("cpp_add", lua_add);

    // lua_.new_usertype<Player>("Player",
    //     sol::constructors<Player()>(),
    //     "health",      &Player::health,
    //     "x",           &Player::x,
    //     "y",           &Player::y,
    //     "move",        &Player::move,
    //     "take_damage", &Player::take_damage
    // );
}

bool ScriptingServer::run_file(std::string_view path) {
    try {
        lua_.safe_script_file(std::string(path));
        return true;
    } catch (const sol::error& e) {
        std::cerr << "[lua] " << e.what() << '\n';
        return false;
    }
}

bool ScriptingServer::run_string(std::string_view code) {
    try {
        lua_.safe_script(std::string(code));
        return true;
    } catch (const sol::error& e) {
        std::cerr << "[lua] " << e.what() << '\n';
        return false;
    }
}

} // namespace meatengine