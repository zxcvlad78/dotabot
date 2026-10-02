#include <meatengine/render/Systems.hpp>
#include <meatengine/sprite/Systems.hpp>
#include <meatengine/ui/Systems.hpp>
#include <meatengine/tilemap/Systems.hpp>
#include <meatengine/debug_tools/common.hpp>

//RenderSystems::enabled = true;

namespace RenderSystems {
    bool enabled = true;

    void render(entt::registry& registry, sf::RenderWindow& window) {
        if (!enabled) return;

        meatengine::TileMapSystems::render(registry, window);
        SpriteSystems::render(registry, window);
		meatengine::ui::Systems::render(registry, window);

    }

}