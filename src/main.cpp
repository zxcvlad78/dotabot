#include <iostream>

#include <meatengine/meatengine.hpp>
#include "GodLike.hpp"

int main() {
	auto main_font = meatengine::ResourceLoader::load<meatengine::Font>("res/font/mainfont.ttf");
	meatengine::ResourceLoader::set_default<meatengine::Font>(main_font);

	auto main_stylebox = meatengine::ResourceLoader::load<meatengine::StyleBox>("res/styleboxes/default.ttf");
	meatengine::ResourceLoader::set_default<meatengine::StyleBox>(main_stylebox);

	meatengine::MainLoop mainloop("GodLike");

	mainloop.run(std::make_unique<GodLike>());

	return 0;
}