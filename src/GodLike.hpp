#pragma once

#include <meatengine/meatengine.hpp>
#include <godlike/DotaGSI.hpp>
#include <godlike/GodLikeBot.hpp>

class GodLike : public meatengine::GameState {
    sf::Vector2f window_sizef;
    entt::entity root_entity;

    std::unique_ptr<DotaGSI> dota_gsi;

    entt::entity hero_label_entity;

public:
    uint8_t click_count = 0;

    void on_enter(sf::RenderWindow& window, entt::registry& registry) override {
        dota_gsi = std::make_unique<DotaGSI>();
        dota_gsi->start();

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

        {hero_label_entity = registry.create(); // test
            auto& transform = registry.emplace<Transform>(hero_label_entity);
			auto& fill_rect = registry.emplace<meatengine::ui::FillRect>(hero_label_entity); {
				fill_rect.foreground = false;
				fill_rect.shape.setSize({1750.f, 2500.f});
				fill_rect.stylebox = meatengine::ResourceLoader::load<meatengine::StyleBox>("res/styleboxes/default.json");
			}
 
            auto& label = registry.emplace<meatengine::ui::Label>(hero_label_entity,
                meatengine::ResourceLoader::load<meatengine::Font>("res/fonts/mainfont.ttf")
            ); {
                label.sf_text->setCharacterSize(16);
                label.sf_text->setString("Hero Name: \nHero Level: ");
            }

        }
    }

    void handle_event(sf::RenderWindow& window, entt::registry& registry, const sf::Event& event) override {
        
    }

    void update(sf::RenderWindow& window, entt::registry& registry, float dt) override {
        auto snapshot = dota_gsi->get_snapshot();

        if (registry.valid(hero_label_entity)) {
            if (registry.all_of<meatengine::ui::Label>(hero_label_entity)) {
                auto& label = registry.get<meatengine::ui::Label>(hero_label_entity);
                label.sf_text->setString(
                    "Hero Name: " + DotaGSIData::Hero::get_name(snapshot) + "\n" +
                    "Hero Level: " + std::to_string(DotaGSIData::Hero::get_level(snapshot)) + "\n"
                );
            }
        }
        
    }
    void update_deferred(sf::RenderWindow& window, entt::registry& registry, float dt) override {

    }
    
    void render(sf::RenderWindow& window, entt::registry& registry, float dt) override {
        
    }
    void render_deferred(sf::RenderWindow& window, entt::registry& registry, float dt) override {
        // update after "render_engine" call
    }

    void on_exit(sf::RenderWindow& window, entt::registry& registry) override {
        if (dota_gsi) {
            dota_gsi->stop();
        }
	}
};
