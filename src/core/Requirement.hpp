#pragma once

#include <optional>
#include <string>
#include <vector>

#include "GameState.hpp"

enum class RequirementType
{
	Flag,
	Skill,
	Counter,
	Feat,
	Item,
	Standing
};

struct Requirement
{
	RequirementType type;	// what kind of info is checked
	std::string key;		// that type's unique name, e.g. flag "intimidated_barkeeper"
	std::optional<int> min;	// optional min value; initialized with !min.has_value()
	std::optional<int> max; // optional max value
	bool expected = true;	// for bool types, flags and feats
};

// checks if a single requirement is given or not according to current GameState
bool requirementMet(const Requirement &req, const GameState &gs);
// check for multiple requirements
bool requirementsMet(const std::vector<Requirement> &req, const GameState &gs);
