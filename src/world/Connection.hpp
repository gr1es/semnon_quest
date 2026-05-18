#pragma once

#include <string>

struct Connection
{
		std::string label;
		/// destination location ID; empty = stay in current location
		std::string destination_location;
		/// destination scene ID; empty = use defaultSceneId() of destination location
		std::string destination_scene;
		// TODO: add "requires" and "effects"
};
