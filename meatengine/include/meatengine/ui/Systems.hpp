#pragma once
#include <entt/entt.hpp>
#include "SFML/Graphics.hpp"

namespace meatengine::ui::Systems {
	void update(entt::registry& reg);
	void render(entt::registry& reg, sf::RenderWindow& window);

	void process_events(entt::registry& reg, const sf::RenderWindow& window);
};