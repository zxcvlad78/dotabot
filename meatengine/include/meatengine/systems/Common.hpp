#pragma once

#include <meatengine/Generic.hpp>
#include <SFML/Graphics/Transform.hpp>

namespace me::systems {
	void movement(entt::registry& reg, float dt);	
}