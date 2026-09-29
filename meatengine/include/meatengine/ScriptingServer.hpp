#pragma once

#include <sol/sol.hpp>
#include <string>
#include <string_view>
#include <meatengine/ResourceLoader.hpp>
#include <meatengine/Resources.hpp>


namespace meatengine {
    class ScriptingServer {
    private:
        sol::state lua_;

    public:
        ScriptingServer() = delete;

		bool run_file(std::string_view path);
		bool run_string(std::string_view code);

		sol::state& lua() { return lua_; }
    };


} // namespace meatengine