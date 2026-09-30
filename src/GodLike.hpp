#pragma once

#include <meatengine/meatengine.hpp>

class GodLike : public meatengine::GameState {
    sf::Vector2f window_sizef;
    entt::entity root_entity;

    entt::entity hero_label_entity;

public:
    uint8_t click_count = 0;

    void on_enter(sf::RenderWindow& window, entt::registry& registry) override {
		{auto entity = registry.create(); //btn 
			auto& transform = registry.emplace<Transform>(entity); {
				transform.position = {255.f, 155.f};
			}

			auto& fill_rect = registry.emplace<meatengine::ui::FillRect>(entity); {
		    	fill_rect.shape.setSize({175.f, 55.f});
                fill_rect.stylebox = meatengine::ResourceLoader::load<meatengine::StyleBox>("res/styleboxes/default.json");
            }

            auto& button = registry.emplace<meatengine::ui::Interactable>(entity); {
                button.on_pressed = [this](entt::registry& r) {
                    click_count++;
                    std::cout << "clicked!!!! " << (int)click_count << std::endl;
                };
            }
            auto& label = registry.emplace<meatengine::ui::Label>(entity,
                meatengine::ResourceLoader::load<meatengine::Font>("res/fonts/mainfont.ttf")
            ); {
                label.sf_text->setString("Buuttttioon!");
            }
		}
    }

    void handle_event(sf::RenderWindow& window, entt::registry& registry, const sf::Event& event) override {
        
    }

    void update(sf::RenderWindow& window, entt::registry& registry, float dt) override {

    }
    void update_deferred(sf::RenderWindow& window, entt::registry& registry, float dt) override {

    }
    
    void render(sf::RenderWindow& window, entt::registry& registry, float dt) override {
        
    }
    void render_deferred(sf::RenderWindow& window, entt::registry& registry, float dt) override {
        // update after "render_engine" call
    }

    void on_exit(sf::RenderWindow& window, entt::registry& registry) override {

	}
};
