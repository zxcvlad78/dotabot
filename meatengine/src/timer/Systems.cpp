#include <meatengine/timer/Components.hpp>
#include <meatengine/timer/Systems.hpp>


namespace TimerSystems {
    void update(entt::registry& registry, float dt) {
        for (auto [e, t] : registry.view<Timer>().each()) {
            if (t.paused) continue;
            if (t.time_left > 0.f) {
                t.time_left -= dt;
            } else {
                t.timeout_func(registry);
                if (!t.one_shot) {
                    t.start();
                } else t.paused = true;
            }
        }
    }
}