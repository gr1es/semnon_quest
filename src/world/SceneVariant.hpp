#pragma once

#include <string>
#include <vector>

#include "Requirement.hpp"

struct SceneVariant
{
		std::vector<Requirement> requirements; // empty = the default entry
		std::string description;
		std::string art;
};
