#include <godlike/systems/Common.hpp>

namespace godlike::systems {
	void player_input(entt::registry& reg, sf::RenderWindow& window) {
		auto view = reg.view<components::MoveSpeed, me::Velocity, components::PlayerInput>();
		
		for (auto [entity, movespeed, velocity] : view.each()) {
			velocity.linear.x = 0.0f;
			velocity.linear.y = 0.0f;
	
			if (me::Console::get_instance().is_visible()) { continue; }
			
			velocity.linear.y -= sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W);
			velocity.linear.y += sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S);
			velocity.linear.x -= sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);
			velocity.linear.x += sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);
			
			velocity.linear.x *= movespeed.value;
			velocity.linear.y *= movespeed.value;
	
			if (velocity.linear.x == 0.0f && velocity.linear.y == 0.0f) continue;
			if (velocity.linear.x != 0.0f && velocity.linear.y != 0.0f) {
				velocity.linear.x *= 0.70710678118f;
				velocity.linear.y *= 0.70710678118f;
			}
			
		}
	}
}