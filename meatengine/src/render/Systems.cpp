#include <meatengine/render/Systems.hpp>
#include <meatengine/sprite/Systems.hpp>
#include <meatengine/ui/Systems.hpp>

//RenderSystems::enabled = true;

namespace RenderSystems {
    bool enabled = true;

    void render(entt::registry& registry, sf::RenderWindow& window) {
        if (!enabled) return;

        SpriteSystems::render(registry, window);
		meatengine::ui::Systems::render(registry, window);

    }

}