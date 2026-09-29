#include "meatengine/lua_bindings/components.hpp"
#include "meatengine/lua_bindings/common.hpp"
#include "meatengine/Resources.hpp"
#include "meatengine/ResourceLoader.hpp"

#include <stdexcept>
#include <string>
#include <vector>

namespace {
	template <typename T>
	void bind_resource(sol::table& table, const std::string& name) {
		table.set_function("load_" + name,
			[](const std::string& path) -> T& {
				auto res = meatengine::ResourceLoader::load<T>(path);
				auto h = res.handle();
				if (!h) throw std::runtime_error("[ResourceLoader] load_" + std::string() + " failed: " + path);
				return *h;
			});

		table.set_function("get_" + name,
			[](const std::string& path) -> T* {
				auto res = meatengine::ResourceLoader::get<T>(path);
				auto h = res.handle();
				return h ? h.get() : nullptr;
			});
	}
}

namespace meatengine::lua_bindings {

void init_resources(sol::state& lua) {
	sol::table rl = lua.create_table();
	lua["ResourceLoader"] = rl;

	bind_resource<meatengine::Font>        (rl, "font");
	bind_resource<meatengine::Texture>     (rl, "texture");
	bind_resource<meatengine::SoundBuffer> (rl, "sound_buffer");
	bind_resource<meatengine::SpriteSheet> (rl, "spritesheet");
	bind_resource<meatengine::TileSet>     (rl, "tileset");
	bind_resource<meatengine::StyleBox>    (rl, "stylebox");

	rl.set_function("load_shader",
		[](const std::string& vs, const std::string& fs) -> meatengine::Shader& {
			auto res = meatengine::ResourceLoader::load<meatengine::Shader>(vs, fs);
			auto h = res.handle();
			if (!h) throw std::runtime_error("load_shader failed: " + vs + " / " + fs);
			return *h;
		});

	rl.set_function("get_shader",
		[](const std::string& vs, const std::string& fs) -> meatengine::Shader* {
			auto res = meatengine::ResourceLoader::get<meatengine::Shader>(vs, fs);
			auto h = res.handle();
			return h ? h.get() : nullptr;
		});

    lua.new_usertype<sf::Vector2u>("Vector2u",
        sol::constructors<sf::Vector2u(), sf::Vector2u(unsigned, unsigned)>(),
        "x", &sf::Vector2u::x,
        "y", &sf::Vector2u::y
    );

    lua.new_usertype<meatengine::Font>("Font", sol::no_constructor);

    lua.new_usertype<meatengine::Texture>("Texture",
        sol::no_constructor,
        "size", sol::property(
            [](meatengine::Texture& t) { return t.res.getSize(); }
        ),
        "smooth", sol::property(
            [](meatengine::Texture& t) { return t.res.isSmooth(); },
            [](meatengine::Texture& t, bool v) { t.res.setSmooth(v); }
        ),
        "repeated", sol::property(
            [](meatengine::Texture& t) { return t.res.isRepeated(); },
            [](meatengine::Texture& t, bool v) { t.res.setRepeated(v); }
        )
    );

    lua.new_usertype<meatengine::SoundBuffer>("SoundBuffer",
        sol::no_constructor,
        "duration", sol::property(
            [](meatengine::SoundBuffer& sb) { return sb.res.getDuration().asSeconds(); }
        )
    );

    lua.new_usertype<meatengine::Shader>("Shader", sol::no_constructor);

    lua.new_usertype<meatengine::Animation::FrameData>("AnimationFrame",
        sol::constructors<meatengine::Animation::FrameData()>(),
        "x", &meatengine::Animation::FrameData::x,
        "y", &meatengine::Animation::FrameData::y,
        "w", &meatengine::Animation::FrameData::w,
        "h", &meatengine::Animation::FrameData::h
    );

    lua.new_usertype<meatengine::Animation>("Animation",
        sol::constructors<meatengine::Animation()>(),
        "name",       &meatengine::Animation::name,
        "fps",        &meatengine::Animation::fps,
        "is_looping", &meatengine::Animation::is_looping,
        "frames",     &meatengine::Animation::frames,
        "duration",   sol::property(
            [](meatengine::Animation& a) { return a.duration(); }
        )
    );

    lua.new_usertype<meatengine::SpriteSheet>("SpriteSheet",
        sol::no_constructor,
        "atlas_width",  &meatengine::SpriteSheet::atlas_width,
        "atlas_height", &meatengine::SpriteSheet::atlas_height,
        "get_animation",
            [](meatengine::SpriteSheet& ss, const std::string& name)
                -> meatengine::Animation* {
                auto it = ss.animations.find(name);
                if (it == ss.animations.end()) return nullptr;
                return &it->second;
            },
        "animation_names",
            [](meatengine::SpriteSheet& ss) {
                std::vector<std::string> names;
                names.reserve(ss.animations.size());
                for (auto& [k, v] : ss.animations) names.push_back(k);
                return names;
            }
    );

    lua.new_usertype<meatengine::TileSet>("TileSet",
        sol::no_constructor,
        "tile_size",     &meatengine::TileSet::tile_size,
        "y_sort_origin", &meatengine::TileSet::y_sort_origin,
        "size",          sol::property(
            [](meatengine::TileSet& ts) { return ts.size(); }
        )
    );

    lua.new_usertype<meatengine::StyleBox>("StyleBox",
        sol::no_constructor,

        "get_color",
            [](meatengine::StyleBox& sb, const std::string& k, sol::optional<sf::Color> def) {
                return sb.get_value<sf::Color>(k, def.value_or(sf::Color::White));
            },
        "get_float",
            [](meatengine::StyleBox& sb, const std::string& k, sol::optional<float> def) {
                return sb.get_value<float>(k, def.value_or(0.f));
            },
        "get_int",
            [](meatengine::StyleBox& sb, const std::string& k, sol::optional<int> def) {
                return sb.get_value<int>(k, def.value_or(0));
            },
        "get_bool",
            [](meatengine::StyleBox& sb, const std::string& k, sol::optional<bool> def) {
                return sb.get_value<bool>(k, def.value_or(false));
            },
        "get_string",
            [](meatengine::StyleBox& sb, const std::string& k, sol::optional<std::string> def) {
                return sb.get_value<std::string>(k, def.value_or(""));
            },

        "set_color",
            [](meatengine::StyleBox& sb, const std::string& k, sf::Color v) {
                sb.set_value<sf::Color>(k, v);
            },
        "set_float",
            [](meatengine::StyleBox& sb, const std::string& k, float v) {
                sb.set_value<float>(k, v);
            },
        "set_int",
            [](meatengine::StyleBox& sb, const std::string& k, int v) {
                sb.set_value<int>(k, v);
            },
        "set_bool",
            [](meatengine::StyleBox& sb, const std::string& k, bool v) {
                sb.set_value<bool>(k, v);
            },
        "set_string",
            [](meatengine::StyleBox& sb, const std::string& k, const std::string& v) {
                sb.set_value<std::string>(k, v);
            }
    );

    lua.set_function("load_font",
        [](const std::string& path) -> meatengine::Font& {
            auto res = meatengine::ResourceLoader::load<meatengine::Font>(path);
            auto h = res.handle();
            if (!h) throw std::runtime_error("load_font failed: " + path);
            return *h;
        });

    lua.set_function("load_texture",
        [](const std::string& path) -> meatengine::Texture& {
            auto res = meatengine::ResourceLoader::load<meatengine::Texture>(path);
            auto h = res.handle();
            if (!h) throw std::runtime_error("load_texture failed: " + path);
            return *h;
        });

    lua.set_function("load_sound_buffer",
        [](const std::string& path) -> meatengine::SoundBuffer& {
            auto res = meatengine::ResourceLoader::load<meatengine::SoundBuffer>(path);
            auto h = res.handle();
            if (!h) throw std::runtime_error("load_sound_buffer failed: " + path);
            return *h;
        });

    lua.set_function("load_spritesheet",
        [](const std::string& path) -> meatengine::SpriteSheet& {
            auto res = meatengine::ResourceLoader::load<meatengine::SpriteSheet>(path);
            auto h = res.handle();
            if (!h) throw std::runtime_error("load_spritesheet failed: " + path);
            return *h;
        });

    lua.set_function("load_tileset",
        [](const std::string& path) -> meatengine::TileSet& {
            auto res = meatengine::ResourceLoader::load<meatengine::TileSet>(path);
            auto h = res.handle();
            if (!h) throw std::runtime_error("load_tileset failed: " + path);
            return *h;
        });

    lua.set_function("load_stylebox",
        [](const std::string& path) -> meatengine::StyleBox& {
            auto res = meatengine::ResourceLoader::load<meatengine::StyleBox>(path);
            auto h = res.handle();
            if (!h) throw std::runtime_error("load_stylebox failed: " + path);
            return *h;
        });
}

} // namespace meatengine::lua_bindings