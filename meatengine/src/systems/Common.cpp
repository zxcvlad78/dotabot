#pragma once
#include <meatengine/systems/Common.hpp>

namespace me::systems {
	void movement(entt::registry& reg, float dt) {
		for (auto [e, t, v] : reg.view<Transform, Velocity>().each()) {
			t.position.x += v.linear.x * dt;
			t.position.y += v.linear.y * dt;

			//t.rotation += sf::Angle::asDegrees(v.angular);
		}
	}
}