#pragma once

#include <string>

struct Connection
{
		/// destination location ID; empty = stay in current location
		std::string destination_location;
		/// destination scene ID; empty = use defaultSceneId() of destination location
		std::string destination_scene;
		std::string label;
		// TODO: add "requires" and "effects"
};
