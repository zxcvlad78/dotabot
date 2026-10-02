#pragma once
#include "Components.hpp"

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>

namespace meatengine::TileMapSystems {
    void update(entt::registry& registry);
    void render(entt::registry& registry, sf::RenderWindow& window);
}