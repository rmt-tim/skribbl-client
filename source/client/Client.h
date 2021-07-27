#pragma once

#include <json/json.hpp>

using namespace nlohmann;

void start_client();

void send_message(json message);

// Returns false when there are no messages in the queue.
bool next_message(json& message);