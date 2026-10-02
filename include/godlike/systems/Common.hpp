#pragma once
#include <meatengine/Generic.hpp>
#include <meatengine/console/Console.hpp>
#include <godlike/components/Common.hpp>

namespace godlike::systems {
	void player_input(entt::registry& reg, sf::RenderWindow& window);
}