#include <iostream>

#include <meatengine/meatengine.hpp>
#include "GodLike.hpp"

int main() {
	meatengine::MainLoop mainloop("GodLike");

	mainloop.run(std::make_unique<GodLike>());

	return 0;
}