#pragma once

#include <string>

#include "Requirement.hpp"
#include "Effect.hpp"

struct Connection
{
		std::string label;
		/// destination location ID; empty = stay in current location
		std::string destination_location;
		/// destination scene ID; empty = use defaultSceneId() of destination location
		std::string destination_scene;

		std::vector<Requirement> requirements;
		std::vector<Effect> effects;
};
