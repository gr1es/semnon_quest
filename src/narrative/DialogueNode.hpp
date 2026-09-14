#pragma once

#include <string>
#include <vector>
#include "DialogueResponse.hpp"

struct DialogueNode
{
	std::string id;	// identifier for node advancement
	std::string text; // what's said
	std::vector<DialogueResponse> responses;	// associated dialogue options
};
