#pragma once
#include "./Components.hpp"

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>

namespace RenderSystems  {
    void render(entt::registry& registry, sf::RenderWindow& window);

    extern bool enabled;
}