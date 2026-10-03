#pragma once

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

#include <string>
#include <functional>

#include <meatengine/Resources.hpp>
#include <meatengine/ResourceLoader.hpp>


namespace me::ui {
	struct UIRoot {};

	struct Padding { float top=0, right=0, bottom=0, left=0; };

	struct Interactable {
		std::function<void(entt::registry&)> on_pressed;
		std::function<void(entt::registry&)> on_hovered;
	};

	struct Disabled {};
	struct Hovered {};
	struct Pressed {};
	struct Focused {};

	enum class LayoutMode { FullRect, Center, Horizontal, Vertical };
	struct Layout {
		LayoutMode mode = LayoutMode::FullRect;
		float spacing = 0.f;
		bool fit_children = false;
	};

	struct FillRect {
		entt::resource<me::StyleBox> stylebox;
		sf::RectangleShape shape;
		bool foreground = true;

		bool dirty = true;

		void update_fill(entt::registry& reg, entt::entity e) {
			if (!dirty) return;
			if (!stylebox) return;


			shape.setOutlineColor(stylebox->get_value<sf::Color>("outline_color", sf::Color::White));
			shape.setOutlineThickness(stylebox->get_value<float>("outline_thickness", 0.0f));

			if (reg.all_of<Disabled>(e))
				shape.setFillColor(stylebox->get_value<sf::Color>("disabled_color", sf::Color::White));
			else if (reg.all_of<Pressed>(e))
				shape.setFillColor(stylebox->get_value<sf::Color>("pressed_color", sf::Color::White));
			else if (reg.all_of<Hovered>(e))
				shape.setFillColor(stylebox->get_value<sf::Color>("hovered_color", sf::Color::White));
			else {
				if (foreground) shape.setFillColor(stylebox->get_value<sf::Color>("foreground_color", sf::Color::White));
				else shape.setFillColor(stylebox->get_value<sf::Color>("background_color", sf::Color::White));
			}
		
			dirty = false;
		}
	};

	struct Label {
		std::optional<sf::Text> sf_text;

		bool dirty = true;

		void update_fill(entt::registry& reg, entt::entity e) {
			if (!sf_text.has_value()) return;
			if (auto* fr = reg.try_get<FillRect>(e)) {
				if (!dirty) return;
				if (!fr->stylebox) return;
	
				if (reg.all_of<Disabled>(e))
					sf_text->setFillColor(fr->stylebox->get_value<sf::Color>("font_disabled_color", sf::Color::White));
				else if (reg.all_of<Pressed>(e))
					sf_text->setFillColor(fr->stylebox->get_value<sf::Color>("font_pressed_color", sf::Color::White));
				else if (reg.all_of<Hovered>(e))
					sf_text->setFillColor(fr->stylebox->get_value<sf::Color>("font_hovered_color", sf::Color::White));
				else 
					sf_text->setFillColor(fr->stylebox->get_value<sf::Color>("font_color", sf::Color::White));
			
			}

			dirty = false;			
		}

		Label() = default; 
		Label(me::Font& font) : sf_text(font.res) { }
	};

	struct Container {

	};

	namespace updating {
		inline std::unordered_set<entt::entity>& unlinking() {
			static std::unordered_set<entt::entity> s;
			return s;
		}

		inline void mark_dirty(entt::registry& r, entt::entity e) {
			if (auto* fr = r.try_get<FillRect>(e)) 
				fr->dirty = true;
			if (auto* lbl = r.try_get<Label>(e))
				lbl->dirty = true;
		}

		inline void install(entt::registry& reg) {
			reg.on_construct<Pressed>().connect<&mark_dirty>();
			reg.on_construct<Hovered>().connect<&mark_dirty>();
			reg.on_construct<Disabled>().connect<&mark_dirty>();
			
			reg.on_destroy<Pressed>().connect<&mark_dirty>();
			reg.on_destroy<Hovered>().connect<&mark_dirty>();
			reg.on_destroy<Disabled>().connect<&mark_dirty>();
		}
	}
}
