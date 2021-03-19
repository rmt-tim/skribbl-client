#include <iostream>
#include <sdlw/sdlw.hpp>
#include <sdlw/image.hpp>
#include <sdlw/ttf.hpp>
#include <json/json.hpp>

int main(int argc, char* argv[])
try {
	nlohmann::json j;
	j["pi"] = 3.14;
	std::cout << j["pi"] << std::endl;

	sdl::subsystem sdl(sdl::subsystem::everything);
	sdl::img::subsystem sdl_image(sdl::img::subsystem::png);
	sdl::ttf::subsystem sdl_ttf;
	sdl::window window("Hello, world", { 100, 100, 800, 600 }, sdl::window::shown);
	sdl::renderer renderer(window, sdl::renderer::accelerated);
	renderer.set_draw_color({ 100, 10, 10, 255 });
	renderer.clear();
	renderer.present();
	sdl::delay(std::chrono::seconds(2));
	return 0;
}
catch (const sdl::error& e) {
	std::cerr << "SDL error: " << e.what();
	return 1;
}
catch (const std::exception& e) {
	std::cerr << "Uncaught exception: " << e.what();
	return 1;
}