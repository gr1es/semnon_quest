#pragma once

#include <string>
#include <vector>
#include "Requirement.hpp"
#include "Effect.hpp"

// note: DialogueOption or DialogueChoice is not optimal since Option and Choice are terms already used elsewhere
struct DialogueResponse
{
	std::string label;	// choice text
	// this is the only spot the dialogue system is influenced by req/eff
	// --> unlocks of responses entirely dependent on individual requirements (no gates on higher levels)
	std::vector<Requirement> requirements;
	std::vector<Effect> effects;
	std::string target_node;	// next node; empty = end of convo
};
