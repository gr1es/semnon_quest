#pragma once

#include <string>
#include <map>
#include "DialogueNode.hpp"

struct Dialogue
{
	std::string id;	// identifier
	std::string start_node; // id of the standard first node
	std::map<std::string, DialogueNode> nodes; // associated nodes, paired with their id, mirroring Location's scenes map
};
