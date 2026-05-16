#pragma once

#include <string>

enum class OptionType
{
	Dialogue,
	Action,
	Move
};

struct Option
{
		std::string label;
		OptionType type;
		/// target = NPC, context or location, depending on option type
		std::string target_id;
		/// flag in GameState needed to make option visible
		std::string required_flag;
};
