#include <entt/entt.hpp>
#include <sol/sol.hpp>
#include <entt/meta/resolve.hpp>

namespace meatengine::lua_bindings {
	void init(sol::state& lua);
	void init_common(sol::state& lua);
	void init_resources(sol::state& lua);
} // namespace meatengine