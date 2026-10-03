#pragma once
#include "Components.hpp"

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>

namespace me::TileMapSystems {
    void update(entt::registry& registry);
    void render(entt::registry& registry, sf::RenderWindow& window);
}