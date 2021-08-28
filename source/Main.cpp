#include <iostream>
#include <thread>

#include <sdlw/sdlw.hpp>
#include <sdlw/image.hpp>
#include <sdlw/ttf.hpp>
#include <json/json.hpp>

#include "../MainFrame.h"

#include "client/Client.h"

using namespace nlohmann;

//void read_messages()
//try {
//	json message;
//	for (;;) {
//		if (next_message(message)) {
//			std::cout << message.dump(2) << "\n" << std::endl;
//		}
//	}
//}
//catch (const std::exception& e) {
//	std::cerr << "Error reading messages: " << e.what() << std::endl;
//}

int main(int argc, char* argv[]) {
	//try {
		start_client();
		MainFrame();

		/*std::thread{ read_messages }.detach();

		std::string line;
		while (std::getline(std::cin, line)) {
			json message;
			try {
				message = json::parse(line);
			}
			catch (const json::parse_error& e) {
				std::cerr << "Invalid JSON. " << e.what() << std::endl;
			}
			send_message(std::move(message));
		}*/

		return 0;
	//}
	/*catch (const sdl::error& e) {
		std::cerr << "SDL error: " << e.what();
		return 1;
	}
	catch (const std::exception& e) {
		std::cerr << "Uncaught exception: " << e.what();
		return 1;
	}*/
}