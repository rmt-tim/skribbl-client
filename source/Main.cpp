#include <iostream>
#include <sdlw/sdlw.hpp>
#include <sdlw/image.hpp>
#include <sdlw/ttf.hpp>
#include <json/json.hpp>
#include <sockets/sockets.hpp>

using namespace nlohmann;

int main(int argc, char* argv[])
try {
	winsock_library winsock;
	tcp_socket server{ "localhost", 9003 };
	server.send("janko\n");
	json response = json::parse(server.receive());
	std::cout << response["tip"] << std::endl;
	for (auto igrac : response["igraci"]) {
		std::cout << igrac << std::endl;
	}

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