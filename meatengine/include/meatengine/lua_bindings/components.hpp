#pragma once
#include <sol/sol.hpp>
#include <entt/entt.hpp>


namespace meatengine::lua_bindings {
	template <typename Component>
    void register_component_methods(sol::usertype<entt::registry>& reg_type, const std::string& name) {
        reg_type.set("add_" + name,
            [](entt::registry& r, entt::entity e) -> Component& {
                return r.get_or_emplace<Component>(e);
            });

        reg_type.set("get_" + name,
            [](entt::registry& r, entt::entity e) -> Component& {
                return r.get<Component>(e);
            });

        reg_type.set("has_" + name,
            [](entt::registry& r, entt::entity e) -> bool {
                return r.all_of<Component>(e);
            });

        reg_type.set("remove_" + name,
            [](entt::registry& r, entt::entity e) {
                r.remove<Component>(e);
            });
    }

    template <typename Component>
    void register_view_method(sol::usertype<entt::registry>& reg_type, const std::string& name) {
        reg_type.set("for_each_" + name,
            [](entt::registry& r, sol::function callback) {
                auto view = r.view<Component>();
                for (auto entity : view) {
                    Component& c = view.get<Component>(entity);
                    callback(entity, std::ref(c));
                }
            });
    }

    template <typename Component>
    void register_component(sol::usertype<entt::registry>& reg_type, const std::string& name) {
        register_component_methods<Component>(reg_type, name);
        register_view_method<Component>(reg_type, name);
    }
}