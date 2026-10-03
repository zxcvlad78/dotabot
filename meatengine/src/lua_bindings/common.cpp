#include "meatengine/lua_bindings/components.hpp"
#include "meatengine/lua_bindings/common.hpp"
#include "meatengine/meatengine.hpp"

#include <entt/entt.hpp>
#include <string>

namespace meatengine::lua_bindings {
void init(sol::state& lua) {
    init_resources(lua);
    init_common(lua);
}

void init_common(sol::state& lua) {
    lua.new_usertype<sf::Vector2f>("Vector2",
        sol::constructors<sf::Vector2f(), sf::Vector2f(float, float)>(),
        "x", &sf::Vector2f::x,
        "y", &sf::Vector2f::y
    );

    lua.new_usertype<sf::Color>("Color",
        sol::constructors<
            sf::Color(),
            sf::Color(std::uint8_t, std::uint8_t, std::uint8_t),
            sf::Color(std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t)
        >(),
        "r", &sf::Color::r,
        "g", &sf::Color::g,
        "b", &sf::Color::b,
        "a", &sf::Color::a
    );

    lua.new_usertype<sf::RectangleShape>("RectangleShape",
        sol::constructors<sf::RectangleShape(), sf::RectangleShape(sf::Vector2f)>(),
        "position", sol::property(
            [](sf::RectangleShape& s) { return s.getPosition(); },
            [](sf::RectangleShape& s, sf::Vector2f p) { s.setPosition(p); }
        ),
        "size", sol::property(
            [](sf::RectangleShape& s) { return s.getSize(); },
            [](sf::RectangleShape& s, sf::Vector2f sz) { s.setSize(sz); }
        ),
        "fill_color", sol::property(
            [](sf::RectangleShape& s) { return s.getFillColor(); },
            [](sf::RectangleShape& s, sf::Color c) { s.setFillColor(c); }
        ),
        "outline_color", sol::property(
            [](sf::RectangleShape& s) { return s.getOutlineColor(); },
            [](sf::RectangleShape& s, sf::Color c) { s.setOutlineColor(c); }
        ),
        "outline_thickness", sol::property(
            [](sf::RectangleShape& s) { return s.getOutlineThickness(); },
            [](sf::RectangleShape& s, float t) { s.setOutlineThickness(t); }
        )
    );

    auto camera_ut = lua.new_usertype<Camera>("Camera",
        sol::constructors<Camera()>(),
        "zoom", &Camera::zoom,
        "smooth", &Camera::smooth
    );

	camera_ut.set_function("is_current", &Camera::is_current);
    camera_ut.set_function("set_current", &Camera::set_current);
    camera_ut.set_function("make_current", &Camera::make_current);
    camera_ut.set_function("get_current", &Camera::get_current);

    auto tilemap_ut = lua.new_usertype<TileMap>("TileMap",
        sol::constructors<TileMap()>(),
        "origin_x", &TileMap::origin_x,
        "origin_y", &TileMap::origin_y,
        "width", &TileMap::width,
        "height", &TileMap::height,
        "dirty", &TileMap::dirty,
        "tiles", &TileMap::tiles,
        "tileset", sol::property(
            [](TileMap& tm) -> meatengine::TileSet* {
                auto h = tm.tileset.handle();
                return h ? h.get() : nullptr;
            },
            [](TileMap& tm, meatengine::TileSet& ts) {
                auto sp = meatengine::ResourceLoader::find_handle(&ts);
                if (!sp) {
                    throw std::runtime_error(
                        "TileMap.tileset: TileSet not from ResourceLoader "
                        "(load it via ResourceLoader.load_tileset first)");
                }
                tm.tileset = entt::resource<meatengine::TileSet>{sp};
                tm.dirty = true;
            }
        )
    );

	tilemap_ut.set_function("load_tiles", &TileMap::load_tiles);
    tilemap_ut.set_function("set_tile", &TileMap::set_tile);
    tilemap_ut.set_function("get_tile", &TileMap::get_tile);

    lua.new_usertype<Transform>("Transform",
        sol::constructors<Transform()>(),
        "position", &Transform::position,
        "rotation", &Transform::rotation,
		"scale", &Transform::scale
    );

    lua.new_usertype<Velocity>("Velocity",
        sol::constructors<Velocity()>(),
        "linear", &Velocity::linear,
        "angular", &Velocity::angular
    );


    lua.new_usertype<meatengine::ui::FillRect>("FillRect",
        sol::constructors<meatengine::ui::FillRect()>(),
        "foreground", &meatengine::ui::FillRect::foreground,
        "dirty", &meatengine::ui::FillRect::dirty,
        "shape", &meatengine::ui::FillRect::shape,

        "stylebox", sol::property(
            [](meatengine::ui::FillRect& fr) -> meatengine::StyleBox* {
                auto h = fr.stylebox.handle();
                return h ? h.get() : nullptr;
            },
            [](meatengine::ui::FillRect& fr, meatengine::StyleBox& sb) {
                auto sp = meatengine::ResourceLoader::find_handle(&sb);
                if (!sp) {
                    throw std::runtime_error(
                        "FillRect.stylebox: StyleBox not from ResourceLoader "
                        "(load it via ResourceLoader.load_stylebox first)");
                }
                fr.stylebox = entt::resource<meatengine::StyleBox>{sp};
                fr.dirty = true;
            }
        )
    );

    lua.new_usertype<meatengine::ui::Interactable>("Interactable",
        sol::constructors<meatengine::ui::Interactable()>()
    );

    lua.new_usertype<meatengine::ui::Label>("Label",
        "text", sol::property(
            [](meatengine::ui::Label& l) -> std::string {
                return l.sf_text->getString().toAnsiString();
            },
            [](meatengine::ui::Label& l, const std::string& s) {
                l.sf_text->setString(sf::String::fromUtf8(s.begin(), s.end()));
                l.dirty = true;
            }
        ),
        "character_size", sol::property(
            [](meatengine::ui::Label& l) { return l.sf_text->getCharacterSize(); },
            [](meatengine::ui::Label& l, unsigned int s) { l.sf_text->setCharacterSize(s); }
        ),
        "color", sol::property(
            [](meatengine::ui::Label& l) { return l.sf_text->getFillColor(); },
            [](meatengine::ui::Label& l, sf::Color c) { l.sf_text->setFillColor(c); }
        ),
        "position", sol::property(
            [](meatengine::ui::Label& l) { return l.sf_text->getPosition(); },
            [](meatengine::ui::Label& l, sf::Vector2f p) { l.sf_text->setPosition(p); }
        ),
        "dirty", &meatengine::ui::Label::dirty
    );

    auto reg_type = lua.new_usertype<entt::registry>("Registry",
        sol::constructors<entt::registry()>()
    );

    reg_type.set("create", [](entt::registry& r) { return r.create(); });
    reg_type.set("destroy", [](entt::registry& r, entt::entity e) { r.destroy(e); });

    register_component<Transform>(reg_type, "Transform");
    register_component<Velocity>(reg_type, "Velocity");
    register_component<TileMap>(reg_type, "TileMap");
    register_component<Camera>(reg_type, "Camera");
    register_component<meatengine::ui::FillRect>(reg_type, "FillRect");
	register_component<meatengine::ui::Label>(reg_type, "Label");
    register_component<meatengine::ui::Interactable>(reg_type, "Interactable");


}


} // namespace meatengine::lua_bindings