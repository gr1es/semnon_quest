#pragma once

#include <string>
#include <vector>

#include "Effect.hpp"
#include "OptionType.hpp"
#include "Requirement.hpp"

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

		std::vector<Requirement> requirements;
		std::vector<Effect> effects;
};
