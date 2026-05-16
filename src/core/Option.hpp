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
		/// Dialogue/Action: NPC or context ID
		std::string target_id;
		/// Move: destination location ID (empty = stay in current location)
		std::string destination_location;
		/// Move: destination scene ID (empty = use defaultSceneId())
		std::string destination_scene;
		/// flag in GameState needed to make option visible
		std::string required_flag;
};
