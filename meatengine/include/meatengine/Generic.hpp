#pragma once

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>

#include <functional>
#include <entt/entt.hpp>
#include <unordered_set>


struct InputState {
    sf::Vector2f mouse_pos;
    bool mouse_down = false;
    bool mouse_just_pressed = false;
    bool mouse_just_released = false;
};

struct ChildOf { entt::entity entity; };
struct ParentOf { entt::entity entity; };

namespace Generic::updating {
    inline std::unordered_set<entt::entity>& unlinking() {
        static std::unordered_set<entt::entity> s;
        return s;
    }


    inline void on_parent_of_created(entt::registry& r, entt::entity e) {
        auto child = r.get<ParentOf>(e).entity;
        if (r.valid(child))
            r.emplace_or_replace<ChildOf>(child, e);
    }

    inline void on_child_of_created(entt::registry& r, entt::entity e) {
        auto parent = r.get<ChildOf>(e).entity;
        if (r.valid(parent))
            r.emplace_or_replace<ParentOf>(parent, e);
    }


    inline void on_parent_of_destroyed(entt::registry& r, entt::entity e) {
        if (unlinking().count(e)) return;
        unlinking().insert(e);
        auto child = r.get<ParentOf>(e).entity;
        if (r.valid(child) && r.all_of<ChildOf>(child)
            && r.get<ChildOf>(child).entity == e)
            r.remove<ChildOf>(child);
        unlinking().erase(e);
    }

    inline void on_child_of_destroyed(entt::registry& r, entt::entity e) {
        if (unlinking().count(e)) return;
        unlinking().insert(e);
        auto parent = r.get<ChildOf>(e).entity;
        if (r.valid(parent) && r.all_of<ParentOf>(parent) && r.get<ParentOf>(parent).entity == e)
            r.remove<ParentOf>(parent);
        unlinking().erase(e);
    }


    inline void install(entt::registry& reg) {
        reg.on_construct<ParentOf>().connect<&on_parent_of_created>();
        reg.on_construct<ChildOf >().connect<&on_child_of_created>();
        reg.on_destroy<ParentOf>().connect<&on_parent_of_destroyed>();
        reg.on_destroy<ChildOf >().connect<&on_child_of_destroyed>();
    }

}

struct Transform {
    sf::Vector2f position;
    sf::Angle rotation_degrees;
    sf::Vector2f scale = {1.f, 1.f};

    Transform& operator=(const Transform& t) {
        if (this != &t) {
            position = t.position;
            rotation_degrees = t.rotation_degrees;
            scale = t.scale;
        }
        return *this;
    }

    Transform& operator=(const Transform* t) {
        if (t != nullptr && this != t) {
            position = t->position;
            rotation_degrees = t->rotation_degrees;
            scale = t->scale;
        }
        return *this;
    }

    static sf::Transform get_global(entt::registry& registry, entt::entity entity) {
        sf::Vector2f pos(0.f, 0.f);
        entt::entity current = entity;
        
        std::vector<entt::entity> chain;
        while (true) {
            chain.push_back(current);
            if (!registry.any_of<ChildOf>(current)) break;
            current = registry.get<ChildOf>(current).entity;
        }
        
        sf::Transform sft;
        for (auto it = chain.rbegin(); it != chain.rend(); ++it) {
            if (registry.all_of<Transform>(*it)) {
                const auto& t = registry.get<Transform>(*it);
                sft.translate(t.position);
                sft.rotate(t.rotation_degrees);
                sft.scale(t.scale);
            }
        }
        
        return sft;
    }
};

struct Offset {
    sf::Vector2f position;

    void center(sf::Vector2f rect_size) {
        position = {
            -static_cast<float>(rect_size.x) / 2.f,
            -static_cast<float>(rect_size.y) / 2.f
        };
    }
};

struct Velocity {
    float x = 0.0f;
    float y = 0.0f;
    bool normalize = true;
};

