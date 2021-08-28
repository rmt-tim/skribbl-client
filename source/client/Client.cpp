#include "Client.h"

#include <algorithm>
#include <iostream>
#include <thread>

#include <rigtorp/SPSCQueue.h>
#include <sockets/sockets.hpp>

static constexpr const char* server_name = "ec2-13-58-58-208.us-east-2.compute.amazonaws.com";
static constexpr short server_port = 9004;

static tcp_socket server{ INVALID_SOCKET };
static rigtorp::SPSCQueue<json> incoming{ 1024 };
static rigtorp::SPSCQueue<json> outgoing{ 1024 };

static void receive_messages()
try {
	std::string buffer;
	for (;;) {
		buffer += server.receive();
		for (;;) {
			auto message_end = std::find(buffer.begin(), buffer.end(), '\n');
			if (message_end == buffer.end()) {
				break;
			}
			incoming.push(json::parse(buffer.begin(), message_end));
			buffer.erase(buffer.begin(), message_end + 1);
		}
	}
}
catch (const std::exception& e) {
	std::cerr << "Receiver stopped. " << e.what() << std::endl;
}

static void send_messages()
try {
	for (;;) {
		if (json* message = outgoing.front()) {
			server.send(message->dump() + '\n');
			outgoing.pop();
		}
	}
}
catch (const std::exception& e) {
	std::cerr << "Sender stopped. " << e.what() << std::endl;
}

void start_client()
{
	static winsock_library winsock;
	server = { server_name, server_port };
	std::thread{ receive_messages }.detach();
	std::thread{ send_messages }.detach();
}

void send_message(json message)
{
	outgoing.push(std::move(message));
}

bool next_message(json& message)
{
	if (json* front = incoming.front()) {
		message = std::move(*front);
		incoming.pop();
		return true;
	}
	return false;
}