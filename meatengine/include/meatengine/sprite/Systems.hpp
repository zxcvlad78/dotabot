#pragma once
#include "./Components.hpp"

namespace SpriteSystems {
    void update(entt::registry& registry, sf::RenderWindow& window, float dt);
    void update_animation(entt::registry& registry, float dt);
    void handle_fullscreen_scale(entt::registry& registry, sf::RenderWindow& window);
    void render(entt::registry& registry, sf::RenderWindow& window);
}