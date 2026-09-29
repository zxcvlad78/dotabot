#pragma once
#include "Components.hpp"
#include <entt/entt.hpp>
#include <meatengine/Resources.hpp>
#include <optional>

struct Sprite {
    sf::Sprite sprite;
    sf::Vector2f offset;
    bool center = false;

    Sprite() = default;
    Sprite(entt::resource<meatengine::Texture> texture) : sprite(texture.handle()->res) { }
};

struct FullScreenScale {
    sf::Vector2f multiplier = {1.f, 1.f};
};

struct SpriteAnimation {
    entt::resource<meatengine::SpriteSheet> spritesheet;
    const meatengine::Animation* current_animation = nullptr;
    
    unsigned int current_frame_idx = 0;
    float time_accumulator = 0.0f;
    bool is_playing = true;
    std::string next_anim = ""; 

    bool play(const std::string& animation_name, const std::string& play_next = "") {
        if (spritesheet && spritesheet->animations.contains(animation_name)) {
            is_playing = true;
            current_animation = &spritesheet->animations.at(animation_name);
            current_frame_idx = 0;
            time_accumulator = 0.0f;
            next_anim = play_next;
            return true;
        }
        return false;
    }
};